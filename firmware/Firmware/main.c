/********************************************************************
 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:		PIC18, PIC24, and PIC32 USB Microcontrollers
 Hardware:		This demo is natively intended to be used on Microchip USB demo
                                boards supported by the MCHPFSUSB stack.  See release notes for
                                support matrix.  This demo can be modified for use on other hardware
                                platforms.
 Complier:  	Microchip C18 (for PIC18), C30 (for PIC24), C32 (for PIC32)
 Company:		Microchip Technology, Inc.

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the �Company�) for its PIC� Microcontroller is intended and
 supplied to you, the Company�s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN �AS IS� CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

 ********************************************************************
 File Description:

 Change History:
  Rev   Description
  ----  -----------------------------------------
  1.0   Initial release
  2.1   Updated for simplicity and to use common
                     coding style
  2.7b  Improvements to USBCBSendResume(), to make it easier to use.
 ********************************************************************/

#ifndef MAIN_C
#define MAIN_C

/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "./HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include <timers.h>
#include <compare.h>

// definitions shared by frontend and backend
#include "../../shared/common.h"

#include "config_bits.h"


#define RX_SIZE 64

/** VARIABLES ******************************************************/
#pragma udata USB_VARIABLES=0x480
unsigned char ReceivedData[RX_SIZE];
unsigned char ToSendDataBuffer[64];
#pragma udata

extern unsigned char ADC_DATA1[];
extern unsigned char ADC_DATA2[];

unsigned char ADC_DATA1[64];
unsigned char ADC_DATA2[64];

#pragma idata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
static void ProcessIO(void);
static void UserInit(void);

void USBCBSendResume(void);
static BYTE ReadSingleADC(void);
static void DAC_Write(WORD daca, WORD dacb);
static void BlinkUSBStatus(void);

/** externals defined in ASM */
extern void SampleFastestSingle(void);
extern void SampleFastestInterleaved(void);
extern void SampleDelayedSingle(WORD delay);
extern void SampleDelayedInterleaved(WORD delay);

typedef enum {
    STATE_READY_RECEIVE,
    STATE_REARM_RX,
    STATE_TRANS_ONE,
    STATE_TRANS_ADC,
    STATE_TRANS_ADC_LAST = STATE_TRANS_ADC + 4
} RXTX_STATE;


#pragma udata
volatile osci_config_t config;
volatile RXTX_STATE state = STATE_READY_RECEIVE;

/** DECLARATIONS ***************************************************/
#pragma code

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/
void main(void) {
    InitializeSystem();

#if defined(USB_INTERRUPT)
    USBDeviceAttach();
#endif

    while (1) {
#if defined(USB_POLLING)
        // Check bus status and service USB interrupts.
        USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        // this function periodically.  This function will take care
        // of processing and responding to SETUP transactions
        // (such as during the enumeration process when you first
        // plug in).  USB hosts require that USB devices should accept
        // and process SETUP packets in a timely fashion.  Therefore,
        // when using polling, this function should be called
        // regularly (such as once every 1.8ms or faster** [see
        // inline code comments in usb_device.c for explanation when
        // "or faster" applies])  In most cases, the USBDeviceTasks()
        // function does not take very long to execute (ex: <100
        // instruction cycles) before it returns.
#endif


        // Application-specific tasks.
        // Application related code may be added here, or in the ProcessIO() function.
        ProcessIO();
    }//end while
}//end main

/********************************************************************
 * Function:        static void InitializeSystem(void)
 *
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void) {
    ADCON1 |= 0x0F; // Default all pins to digital

#if defined(PIC18F87J50_PIM) || defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1) || defined(PIC18F47J53_PIM)
    //On the PIC18F87J50 Family of USB microcontrollers, the PLL will not power up and be enabled
    //by default, even if a PLL enabled oscillator configuration is selected (such as HS+PLL).
    //This allows the device to power up at a lower initial operating frequency, which can be
    //advantageous when powered from a source which is not gauranteed to be adequate for 48MHz
    //operation.  On these devices, user firmware needs to manually set the OSCTUNE<PLLEN> bit to
    //power up the PLL.
    {
        unsigned int pll_startup_counter = 600;
        OSCTUNEbits.PLLEN = 1; //Enable the PLL and wait 2+ms until the PLL locks before enabling USB module
        while (pll_startup_counter--);
    }
    //Device switches over automatically to PLL output after PLL is locked and ready.
#endif

#if defined(PIC18F87J50_PIM)
    //Configure all I/O pins to use digital input buffers.  The PIC18F87J50 Family devices
    //use the ANCONx registers to control this, which is different from other devices which
    //use the ADCON1 register for this purpose.
    WDTCONbits.ADSHR = 1; // Select alternate SFR location to access ANCONx registers
    ANCON0 = 0xFF; // Default all pins to digital
    ANCON1 = 0xFF; // Default all pins to digital
    WDTCONbits.ADSHR = 0; // Select normal SFR locations
#endif


    //	The USB specifications require that USB peripheral devices must never source
    //	current onto the Vbus pin.  Additionally, USB peripherals should not source
    //	current on D+ or D- when the host/hub is not actively powering the Vbus line.
    //	When designing a self powered (as opposed to bus powered) USB peripheral
    //	device, the firmware should make sure not to turn on the USB module and D+
    //	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
    //	firmware needs some means to detect when Vbus is being powered by the host.
    //	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
    // 	can be used to detect when Vbus is high (host actively powering), or low
    //	(host is shut down or otherwise not supplying power).  The USB firmware
    // 	can then periodically poll this I/O pin to know when it is okay to turn on
    //	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
    //	peripheral device, it is not possible to source current on D+ or D- when the
    //	host is not actively providing power on Vbus. Therefore, implementing this
    //	bus sense feature is optional.  This firmware can be made to use this bus
    //	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
    //	HardwareProfile.h file.
#if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
#endif

    //	If the host PC sends a GetStatus (device) request, the firmware must respond
    //	and let the host know if the USB peripheral device is currently bus powered
    //	or self powered.  See chapter 9 in the official USB specifications for details
    //	regarding this request.  If the peripheral device is capable of being both
    //	self and bus powered, it should not return a hard coded value for this request.
    //	Instead, firmware should check if it is currently self or bus powered, and
    //	respond accordingly.  If the hardware has been configured like demonstrated
    //	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
    //	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2"
    //	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
    //	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
    //	to it in HardwareProfile.h.
#if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN; // See HardwareProfile.h
#endif

    UserInit();

    USBDeviceInit(); //usb_device.c.  Initializes USB module SFRs and firmware
    //variables to known states.
}//end InitializeSystem



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

    switch (config.inputChannel) {
        case ADC_ch1: SelectInputChannel1();
            break;
        case ADC_ch2: SelectInputChannel2();
            break;
        case ADC_triggerLevel: SelectInputChannelT();
            break;
    }
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

    config.sampleDelay = 1;

    CallDelay();
    //    SampleFastestInterleaved();

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

/********************************************************************
 * Function:        void ProcessIO(void)
 *
 * Overview:        This function is a place holder for other user
 *                  routines. It is a mixture of both USB and
 *                  non-USB tasks.
 *
 * Note:            None
 *******************************************************************/


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

#define ACKNOLEDGE(x) { ToSendDataBuffer[0] = x; }
    const BYTE nack = 0x00;
    const BYTE ack = ReceivedData[0] + 1;

    // default: single transfer answer
    state = STATE_TRANS_ONE;

    switch (ReceivedData[0]) //Look at the data the host sent, to see what kind of application specific command it sent.
    {

        case OP_SET_CONFIG:
            memcpy((void*) &config, (void*) &ReceivedData[1], sizeof (osci_config_t));

            ToSendDataBuffer[1] = sizeof (osci_config_t);
            memcpy((void*) &ToSendDataBuffer[2], (void*) &config, sizeof (osci_config_t));

            ApplyOsciConfig();
            ACKNOLEDGE(ack);
            break;

        case OP_SAMPLE_DUMMY_TRI:
        {
            WORD cnt, cnt2 = 0;
            state = STATE_TRANS_ADC;

            // DisableInterrupts();
            for (cnt = 0; cnt < 64; cnt++)
                ToSendDataBuffer[cnt] = cnt2++;
            for (cnt = 0; cnt < 64; cnt++)
                ReceivedData[cnt] = cnt2++;
            for (cnt = 0; cnt < 64; cnt++)
                ADC_DATA1[cnt] = cnt2++;
            for (cnt = 0; cnt < 64; cnt++)
                ADC_DATA2[cnt] = cnt2++;

        }
            ACKNOLEDGE(ack);
            break;


        case OP_SAMPLE_DUMMY_ZERO:
        {
            WORD cnt;
            state = STATE_TRANS_ADC;

            // DisableInterrupts();
            for (cnt = 0; cnt < 64; cnt++)
                ToSendDataBuffer[cnt] = 0;
            for (cnt = 0; cnt < 64; cnt++)
                ReceivedData[cnt] = 0;
            for (cnt = 0; cnt < 64; cnt++)
                ADC_DATA1[cnt] = 0;
            for (cnt = 0; cnt < 64; cnt++)
                ADC_DATA2[cnt] = 0;

        }
            ACKNOLEDGE(ack);
            break;

        case OP_SAMPLE_DUMMY_MID:
        {
            WORD cnt;
            state = STATE_TRANS_ADC;

            // DisableInterrupts();
            for (cnt = 0; cnt < 64; cnt++) 
                ToSendDataBuffer[cnt] = 0x80;
            for (cnt = 0; cnt < 64; cnt++)
                ReceivedData[cnt] = 0x80;
            for (cnt = 0; cnt < 64; cnt++) 
                ADC_DATA1[cnt] = 0x80;
            for (cnt = 0; cnt < 64; cnt++) 
                ADC_DATA2[cnt] = 0x80;

        }
            ACKNOLEDGE(ack);
            break;


        case OP_READ_SINGLE:
            ToSendDataBuffer[1] = ReadSingleADC();
            ACKNOLEDGE(ack);
            break;

        case OP_SAMPLE_SINGLE:
        {
            //sample without delay

            state = STATE_TRANS_ADC;

            WaitForTrigger();

            DisableInterrupts();
            O_ADC_CS = 0;
            if (config.sampleDelay == 0) {

                SampleFastestSingle();
            } else {
                SampleDelayedSingle(config.sampleDelay);
            }

            O_ADC_CS = 1;
            EnableInterrupts();


            LedOn();
        }
            ACKNOLEDGE(ack);
            break;

        case OP_SAMPLE_INTERLEAVED:
        {
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


            ACKNOLEDGE(ack);
            break;


        case OP_PING:
            //ping
            ACKNOLEDGE(ack);
            break;

        default:
            // not implemented: return some nack
            ACKNOLEDGE(nack);
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



    switch (state) {
        case STATE_READY_RECEIVE:
            if (!HIDRxHandleBusy(USBOutHandle))
                HandleRequest();
            break;
        case STATE_REARM_RX:
            //Re-arm the OUT endpoint for the next packet
            USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedData, RX_SIZE);
            state = STATE_READY_RECEIVE;
            break;
        case STATE_TRANS_ONE:
            if (!HIDTxHandleBusy(USBInHandle)) {
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = STATE_REARM_RX;
            }
            break;
        case STATE_TRANS_ADC:
            if (!HIDTxHandleBusy(USBInHandle)) {
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = state + 1;
            }
            break;

        case STATE_TRANS_ADC + 1:
            if (!HIDTxHandleBusy(USBInHandle)) {
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ReceivedData[0], 64);
                state = state + 1;
            }
            break;

        case STATE_TRANS_ADC + 2:
            if (!HIDTxHandleBusy(USBInHandle)) {
                memcpy((BYTE*) & ToSendDataBuffer[0], (void*) &ADC_DATA1[0], 64);
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = state + 1;
            }
            break;
        case STATE_TRANS_ADC + 3:
            if (!HIDTxHandleBusy(USBInHandle)) {
                memcpy((BYTE*) & ToSendDataBuffer[0], (void*) &ADC_DATA2[0], 64);
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = STATE_REARM_RX;
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

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void) {
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.

    if (USBSuspendControl == 1) {
    } else {
        if (USBDeviceState == DETACHED_STATE) {
        } else if (USBDeviceState == ATTACHED_STATE) {
        } else if (USBDeviceState == POWERED_STATE) {
        } else if (USBDeviceState == DEFAULT_STATE) {
        } else if (USBDeviceState == ADDRESS_STATE) {
        } else if (USBDeviceState == CONFIGURED_STATE) {
        }
    }

}//end BlinkUSBStatus




// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.

/******************************************************************************
 * Function:        void USBCBSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Call back that is invoked when a USB suspend is detected
 *
 * Note:            None
 *****************************************************************************/
void USBCBSuspend(void) {
    //Example power saving code.  Insert appropriate code here for the desired
    //application behavior.  If the microcontroller will be put to sleep, a
    //process similar to that shown below may be used:

    //ConfigureIOPinsForLowPower();
    //SaveStateOfAllInterruptEnableBits();
    //DisableAllInterruptEnableBits();
    //EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
    //Sleep();
    //RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
    //RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

    //IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is
    //cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause
    //things to not work as intended.


#if defined(__C30__)
    //This function requires that the _IPL level be something other than 0.
    //  We can set it here to something other than
#ifndef DSPIC33E_USB_STARTER_KIT
    _IPL = 1;
    USBSleepOnSuspend();
#endif
#endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0

void __attribute__((interrupt)) _USB1Interrupt(void) {
#if !defined(self_powered)
    if (U1OTGIRbits.ACTVIF) {
        IEC5bits.USB1IE = 0;
        U1OTGIEbits.ACTVIE = 0;
        IFS5bits.USB1IF = 0;

        //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
        USBClearInterruptFlag(USBIdleIFReg, USBIdleIFBitNum);
        //USBSuspendControl = 0;
    }
#endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void) {
    // If clock switching or other power savings measures were taken when
    // executing the USBCBSuspend() function, now would be a good time to
    // switch back to normal full power run mode conditions.  The host allows
    // a few milliseconds of wakeup time, after which the device must be
    // fully back to normal, and capable of receiving and processing USB
    // packets.  In order to do this, the USB module must receive proper
    // clocking (IE: 48MHz clock must be available to SIE for full speed USB
    // operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void) {
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void) {
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

    // Typically, user firmware does not need to do anything special
    // if a USB error occurs.  For example, if the host sends an OUT
    // packet to your device, but the packet gets corrupted (ex:
    // because of a bad connection, or the user unplugs the
    // USB cable during the transmission) this will typically set
    // one or more USB error interrupt flags.  Nothing specific
    // needs to be done however, since the SIE will automatically
    // send a "NAK" packet to the host.  In response to this, the
    // host will normally retry to send the packet again, and no
    // data loss occurs.  The system will typically recover
    // automatically, without the need for application firmware
    // intervention.

    state = STATE_READY_RECEIVE;
    // Nevertheless, this callback function is provided, such as
    // for debugging purposes.
}

/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *******************************************************************/
void USBCBCheckOtherReq(void) {
    USBCheckHIDRequest();
}//end

/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *******************************************************************/
void USBCBStdSetDscHandler(void) {
    // Must claim session ownership if supporting this request
}//end

/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void) {
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Re-arm the OUT endpoint for the next packet
    USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedData, RX_SIZE);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function will only be able to wake up the host if
 *                  all of the below are true:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *                  If the host has not armed the device to perform remote wakeup,
 *                  then this function will return without actually performing a
 *                  remote wakeup sequence.  This is the required behavior, 
 *                  as a USB device that has not been armed to perform remote 
 *                  wakeup must not drive remote wakeup signalling onto the bus;
 *                  doing so will cause USB compliance testing failure.
 *                  
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            This function does nothing and returns quickly, if the USB
 *                  bus and host are not in a suspended condition, or are 
 *                  otherwise not in a remote wakeup ready state.  Therefore, it
 *                  is safe to optionally call this function regularly, ex: 
 *                  anytime application stimulus occurs, as the function will
 *                  have no effect, until the bus really is in a state ready
 *                  to accept remote wakeup. 
 *
 *                  When this function executes, it may perform clock switching,
 *                  depending upon the application specific code in 
 *                  USBCBWakeFromSuspend().  This is needed, since the USB
 *                  bus will no longer be suspended by the time this function
 *                  returns.  Therefore, the USB module will need to be ready
 *                  to receive traffic from the host.
 *
 *                  The modifiable section in this routine may be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of ~3-15 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at least 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void) {
    static WORD delay_count;

    //First verify that the host has armed us to perform remote wakeup.
    //It does this by sending a SET_FEATURE request to enable remote wakeup,
    //usually just before the host goes to standby mode (note: it will only
    //send this SET_FEATURE request if the configuration descriptor declares
    //the device as remote wakeup capable, AND, if the feature is enabled
    //on the host (ex: on Windows based hosts, in the device manager 
    //properties page for the USB device, power management tab, the 
    //"Allow this device to bring the computer out of standby." checkbox 
    //should be checked).
    if (USBGetRemoteWakeupStatus() == TRUE) {
        //Verify that the USB bus is in fact suspended, before we send
        //remote wakeup signalling.
        if (USBIsBusSuspended() == TRUE) {
            USBMaskInterrupts();

            //Clock switch to settings consistent with normal USB operation.
            USBCBWakeFromSuspend();
            USBSuspendControl = 0;
            USBBusIsSuspended = FALSE; //So we don't execute this code again,
            //until a new suspend condition is detected.

            //Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            //device must continuously see 5ms+ of idle on the bus, before it sends
            //remote wakeup signalling.  One way to be certain that this parameter
            //gets met, is to add a 2ms+ blocking delay here (2ms plus at 
            //least 3ms from bus idle to USBIsBusSuspended() == TRUE, yeilds
            //5ms+ total delay since start of idle).
            delay_count = 3600U;
            do {
                delay_count--;
            } while (delay_count);

            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1; // Start RESUME signaling
            delay_count = 1800U; // Set RESUME line for 1-13 ms
            do {
                delay_count--;
            } while (delay_count);
            USBResumeControl = 0; //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}

/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size) {
    switch ((INT) event) {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            //Add application specific callback task or callback function here if desired.
            //The EVENT_TRANSFER_TERMINATED event occurs when the host performs a CLEAR
            //FEATURE (endpoint halt) request on an application endpoint which was 
            //previously armed (UOWN was = 1).  Here would be a good place to:
            //1.  Determine which endpoint the transaction that just got terminated was 
            //      on, by checking the handle value in the *pdata.
            //2.  Re-arm the endpoint if desired (typically would be the case for OUT 
            //      endpoints).
            break;
        default:
            break;
    }
    return TRUE;
}

/** EOF main.c *************************************************/
#endif
