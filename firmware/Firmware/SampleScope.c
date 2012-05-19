/*

SampleScope: A humble USB oscilloscope
Copyright (C) 2012 mru

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "./USB/usb.h"
#include "./HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include <timers.h>
#include <compare.h>
#include "../../shared/common.h"



extern unsigned char* ToSendDataBuffer;
extern unsigned char* ReceivedData;
extern USB_HANDLE USBOutHandle;
extern USB_HANDLE USBInHandle;


#pragma udata gpr2
BYTE B_ADC_0[256];
#pragma udata gpr5
BYTE B_ADC_1[256];
#pragma udata gpr6
BYTE B_ADC_2[256];
#pragma udata gpr7
BYTE B_ADC_3[256];

#pragma udata 

WORD_VAL TransmitBufferCounter;

#pragma idata


/** PRIVATE PROTOTYPES *********************************************/
extern void ProcessIO(void);
extern void UserInit(void);

static BYTE ReadSingleADC(void);
static void DAC_Write(WORD daca, WORD dacb);

/** externals defined in ASM */
extern void SampleFastestSingle(void);
extern void SampleFastestInterleaved(void);
extern void SampleDelayedSingle(WORD delay);
extern void SampleDelayedInterleaved(WORD delay);

typedef enum {
    STATE_READY_RECEIVE,
    STATE_TRANS_ONE,
    STATE_TRANS_ADC,
    STATE_LAST
} RXTX_STATE;


#pragma udata
osci_config_t config;
RXTX_STATE state = STATE_READY_RECEIVE;

/** DECLARATIONS ***************************************************/
#pragma code


void resetState(void) {
    state = STATE_READY_RECEIVE;
}


#define LedBlink2() do { OpenCompare1(COM_INT_OFF & COM_TOGG_MATCH, 0x0000); } while (0)
#define LedBlink() { CCP1CONbits.CCP1M = 0b0010; CCPR1 = 0xffff; }
#define LedOn() { CloseCompare1(); O_LED = 1; }
#define LedOff()  { CloseCompare1(); O_LED = 0; }

#define ApplyTriggerLevel(level) { \
    O_DAC_Gain_CS = 1; \
    O_DAC_Trigger_CS = 0; \
    DAC_Write(level, 0x0fff);\
    O_DAC_Trigger_CS = 1;\
}

#define ApplyGain(gainA, gainB) { \
    O_DAC_Trigger_CS = 1; \
    O_DAC_Gain_CS = 0; \
    DAC_Write(gainA, gainB); \
    O_DAC_Gain_CS = 1; \
}

#define SelectInputChannel1() { O_ADC_A0 = 0; O_ADC_A1 = 0; }
#define SelectInputChannel2() { O_ADC_A0 = 1; O_ADC_A1 = 0; }
#define SelectInputChannelT() { O_ADC_A0 = 0; O_ADC_A1 = 1; }

void ApplyOsciConfig(void) {

    // triggerLevel, triggerMode, sampleDelay
    // are used as variables, no apply necessary

    O_ACDC_Ch1 = config.acdcCh1;
    O_ACDC_Ch2 = config.acdcCh2;
    ApplyTriggerLevel(config.triggerLevel);
    ApplyGain(config.gainCh1, config.gainCh2);

    Nop();
}

extern void CallDelay(void);

void UserInit(void) {
    LATB &= ~((1 << 2) | 1 << 3 | 1 << 4);
    LATD = 0x0;
    LATE &= ~(1 << 0 | 1 << 1 | 1 << 2);

    TRISB &= ~((1 << 2) | 1 << 3 | 1 << 4);
    TRISE &= ~(1 << 0 | 1 << 1 | 1 << 2);
    TRISD = 0xff;
    TRISC &= ~(1 << 2);

    TRISA &= ~(1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);


    O_ADC_CS = 1;
    O_ADC_RD = 1;

    O_DAC_Trigger_CS = 1;
    O_DAC_Gain_CS = 1;
    O_DAC_SCLK = 0;
    O_DAC_DIN = 0;


    InitOsciConfig(config);
    ApplyOsciConfig();

#if 0
    // this makes the device dysfuntional!
    OpenTimer1(T1_OSC1EN_OFF & T1_PS_1_8 & T1_SOURCE_INT & T1_SYNC_EXT_OFF);
#else

    T1CONbits.T1OSCEN = 0;

    T1CONbits.T1CKPS = 0b11;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
#endif

    LedBlink();

    //    INTCON2bits.RBPU = 0;

    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;
}//end UserInit

/** DAC_Write */
void DAC_Write(WORD daca, WORD dacb) {
    WORD_VAL daca_v = daca;
    WORD_VAL dacb_v = dacb;
    int i;
    for (i = 12 - 1; i >= 0; i--) {
        O_DAC_DIN = (dacb_v.Val >> i) & 1;
        O_DAC_SCLK = 0;
        O_DAC_SCLK = 1;
    }
    for (i = 12 - 1; i >= 0; i--) {
        O_DAC_DIN = (daca_v.Val >> i) & 1;
        O_DAC_SCLK = 0;
        O_DAC_SCLK = 1;
    }
}

static void WaitForTrigger(void) {
    WORD timeout = 0xffff;
    LedBlink();

    if (config.triggerChannel == TRIGGER_CH_1) {
        if (config.triggerMode == TRIGGER_RISING) {
            while (I_TriggerCh1 && timeout-- > 0);
            while (!I_TriggerCh1 && timeout-- > 0);
        } else {
            while (!I_TriggerCh1 && timeout-- > 0);
            while (I_TriggerCh1 && timeout-- > 0);
        }
    } else if (config.triggerChannel == TRIGGER_CH_2) {
        if (config.triggerMode == TRIGGER_RISING) {
            while (I_TriggerCh2 && timeout-- > 0);
            while (!I_TriggerCh2 && timeout-- > 0);
        } else {
            while (!I_TriggerCh2 && timeout-- > 0);
            while (I_TriggerCh2 && timeout-- > 0);
        }
    }

    LedOff();
}

#define EnableInterrupts() { INTCONbits.GIE = 1; }
#define DisableInterrupts() { INTCONbits.GIE = 0; }

void HandleRequest(void) {

    // save acknoledge for later
    const BYTE ack = ReceivedData[0] + 1;
    opcode_decoder_t* decoder = (opcode_decoder_t*) & ReceivedData[0];
    result_encoder_t* encoder = (result_encoder_t*) & ToSendDataBuffer[0];

#define ACKNOLEDGE(x) { encoder->ack = (x); }
#define ACK() ACKNOLEDGE(ack)
#define NACK() ACKNOLEDGE(0x00)

    // default: single transfer answer
    state = STATE_TRANS_ONE;

    switch (ReceivedData[0]) {

        case OP_GET_INFO:
        {
            device_info_t info;
            info.version = 1;
            info.sizeInfo = sizeof (device_info_t);
            info.sizeConfig = sizeof (osci_config_t);
            info.bufferCount = 16;
            info.bufferSize = 64;

            encoder->args.get_info_args = info;

            ACK();

        }
            break;
        case OP_SET_CONFIG:
            config = decoder->args.set_config_args;

            ApplyOsciConfig();
            ACK();
            break;

        case OP_READ_SINGLE:
            ToSendDataBuffer[1] = ReadSingleADC();
            ACK();
            break;

        case OP_SAMPLE_SINGLE:
        {
            TransmitBufferCounter.Val = 0;
            state = STATE_TRANS_ADC;


            switch (decoder->args.sample_single_args.testSignal) {

                case TestSignal_Off:

                    switch (decoder->args.sample_single_args.inputChannel) {
                        case ADC_ch1: SelectInputChannel1();
                            break;
                        case ADC_ch2: SelectInputChannel2();
                            break;
                        case ADC_triggerLevel: SelectInputChannelT();
                            break;
                    }


                    DisableInterrupts();
                    O_ADC_CS = 0;
                    if (config.sampleDelay == 0) {
                        WaitForTrigger();
                        SampleFastestSingle();
                    } else {
                        WaitForTrigger();
                        SampleDelayedSingle(config.sampleDelay);
                    }

                    O_ADC_CS = 1;
                    EnableInterrupts();

                    break;

                case TestSignal_Tri:
                {
                    WORD cnt, cnt2 = 0;
                    for (cnt = 0; cnt < 256; cnt++) B_ADC_0[cnt] = cnt2++;
                    for (cnt = 0; cnt < 256; cnt++) B_ADC_1[cnt] = cnt2++;
                    for (cnt = 0; cnt < 256; cnt++) B_ADC_2[cnt] = cnt2++;
                    for (cnt = 0; cnt < 256; cnt++) B_ADC_3[cnt] = cnt2++;

                }
                    break;
                    /*
                case TestSignal_Tri_2:
                {
                    WORD cnt, i;
                    for (i = 0; i < 4; i++)
                        for (cnt = 0; cnt < 64; cnt++)
                            TX_BUFFER(i)[cnt] = cnt;
                }

                    break;
                case TestSignal_Zero:
                {
                    WORD cnt, i;
                    for (i = 0; i < 4; i++)
                        for (cnt = 0; cnt < 64; cnt++)
                            TX_BUFFER(i)[cnt] = 0;
                }


                    break;
                case TestSignal_Mid:
                {
                    WORD cnt, i;
                    for (i = 0; i < 4; i++)
                        for (cnt = 0; cnt < 64; cnt++)
                            TX_BUFFER(i)[cnt] = 0x80;

                }
                    break;
                     * */
            }

            LedOn();
        }
            B_ADC_0[0] = ack;

            ACK();
            break;

        case OP_SAMPLE_INTERLEAVED:
        {
            TransmitBufferCounter.Val = 0;
            state = STATE_TRANS_ADC;

            DisableInterrupts();
            WaitForTrigger();

            O_ADC_CS = 0;
            if (config.sampleDelay == 0) {
                SampleFastestInterleaved();
            } else {
                SampleDelayedInterleaved(config.sampleDelay);
            }
            EnableInterrupts();
            O_ADC_CS = 1;

            LedOn();
        }


            ACK();
            break;


        case OP_PING:
            //ping
            ACK();
            break;

        default:
            // not implemented: return some nack
            NACK();
            break;
    }

}

void ProcessIO(void) {


    //BlinkUSBStatus();

    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) {

        // reset RXTX state
        state = STATE_READY_RECEIVE;
        return;
    }

#define RearmRX() { USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedData[0], 64); }
#define RearmTX() { USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64); }
#define Tx_buf(_b) { memcpy( (void*) &ToSendDataBuffer[0], (void*)(_b), 64); RearmTX(); }

    switch (state) {
        case STATE_READY_RECEIVE:

            if (!HIDRxHandleBusy(USBOutHandle))
                HandleRequest();
            break;

        case STATE_TRANS_ONE:

            if (!HIDTxHandleBusy(USBInHandle)) {
                RearmTX();
                RearmRX();
                state = STATE_READY_RECEIVE;
            }
            break;

        case STATE_TRANS_ADC:

            if (!HIDTxHandleBusy(USBInHandle)) {

                /*if (TransmitBufferCounter.v[0] == 0 && TransmitBufferCounter.v[1] == 0) {
                    TransmitBufferCounter.v[0]++;
                    RearmTX();
                } else */
                if (TransmitBufferCounter.v[1] == 4) {
                    RearmRX();
                    TransmitBufferCounter.Val = 0;
                    state = STATE_READY_RECEIVE;
                } else {
                    switch (TransmitBufferCounter.v[1]) {
                        case 0:
                            Tx_buf(&B_ADC_0[TransmitBufferCounter.v[0] * 64]);
                            break;
                        case 1:
                            Tx_buf(&B_ADC_1[TransmitBufferCounter.v[0] * 64]);
                            break;
                        case 2:
                            Tx_buf(&B_ADC_2[TransmitBufferCounter.v[0] * 64]);
                            break;
                        case 3:
                            Tx_buf(&B_ADC_3[TransmitBufferCounter.v[0] * 64]);
                            break;
                    }

                    TransmitBufferCounter.v[0]++;
                    if (TransmitBufferCounter.v[0] == 4) {
                        TransmitBufferCounter.v[0] = 0;
                        TransmitBufferCounter.v[1]++;
                    }
                }
            }
            break;

    }

}//end ProcessIO

BYTE ReadSingleADC(void) {
    BYTE w;
    O_ADC_CS = O_ADC_RD = 0;
    //while (PORTBbits.RB1);
    w = PORTD;
    O_ADC_CS = O_ADC_RD = 1;
    return w;
}

