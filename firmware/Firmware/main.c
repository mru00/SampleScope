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

/** CONFIGURATION **************************************************/

#pragma config PLLDIV   = 5         // (20 MHz crystal on PICDEM FS USB board)
#pragma config CPUDIV   = OSC1_PLL2
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config FCMEN    = OFF
#pragma config IESO     = OFF
#pragma config PWRT     = ON
#pragma config BOR      = ON
#pragma config BORV     = 28
#pragma config VREGEN   = ON      //USB Voltage Regulator
#pragma config WDT      = OFF
#pragma config WDTPS    = 32768
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
//      #pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
#pragma config XINST    = OFF       // Extended Instruction Set
#pragma config CP0      = OFF
#pragma config CP1      = OFF
//      #pragma config CP2      = OFF
//      #pragma config CP3      = OFF
#pragma config CPB      = OFF
//      #pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
//      #pragma config WRT2     = OFF
//      #pragma config WRT3     = OFF
#pragma config WRTB     = OFF       // Boot Block Write Protection
#pragma config WRTC     = OFF
//      #pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
//      #pragma config EBTR2    = OFF
//      #pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF


#define RX_SIZE 64

/** VARIABLES ******************************************************/
#pragma udata USB_VARIABLES=0x480
unsigned char ReceivedDataBuffer[RX_SIZE];
unsigned char ToSendDataBuffer[64];
#pragma udata

extern unsigned char ADC_DATA1[];
extern unsigned char ADC_DATA2[];

unsigned char ADC_DATA1[64];
unsigned char ADC_DATA2[64];

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
static void ProcessIO(void);
static void UserInit(void);

void USBCBSendResume(void);
static BYTE ReadSingleADC(void);
static void DAC_Write(WORD_VAL daca, WORD_VAL dacb);
static void BlinkUSBStatus(void);

/** externals defined in ASM */
extern void SampleNoDelaySingleChannel(void);











void YourHighPriorityISRCode();
void YourLowPriorityISRCode();
extern void USBDeviceTasks(void);






/** VECTOR REMAPPING ***********************************************/
#if defined(__18CXX)
//On PIC18 devices, addresses 0x00, 0x08, and 0x18 are used for
//the reset, high priority interrupt, and low priority interrupt
//vectors.  However, the current Microchip USB bootloader
//examples are intended to occupy addresses 0x00-0x7FF or
//0x00-0xFFF depending on which bootloader is used.  Therefore,
//the bootloader code remaps these vectors to new locations
//as indicated below.  This remapping is only necessary if you
//wish to program the hex file generated from this project with
//the USB bootloader.  If no bootloader is used, edit the
//usb_config.h file and comment out the following defines:
//#define PROGRAMMABLE_WITH_USB_HID_BOOTLOADER
//#define PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER

#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)
#define REMAPPED_RESET_VECTOR_ADDRESS			0x1000
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x1008
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x1018
#elif defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
#else
#define REMAPPED_RESET_VECTOR_ADDRESS			0x00
#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x08
#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x18
#endif

#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
extern void _startup(void); // See c018i.c in your C18 compiler dir
#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS

void _reset(void) {
    _asm goto _startup _endasm
}
#endif
#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS

void Remapped_High_ISR(void) {
    _asm goto YourHighPriorityISRCode _endasm
}
#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS

void Remapped_Low_ISR(void) {
    _asm goto YourLowPriorityISRCode _endasm
}

#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)
//Note: If this project is built while one of the bootloaders has
//been defined, but then the output hex file is not programmed with
//the bootloader, addresses 0x08 and 0x18 would end up programmed with 0xFFFF.
//As a result, if an actual interrupt was enabled and occured, the PC would jump
//to 0x08 (or 0x18) and would begin executing "0xFFFF" (unprogrammed space).  This
//executes as nop instructions, but the PC would eventually reach the REMAPPED_RESET_VECTOR_ADDRESS
//(0x1000 or 0x800, depending upon bootloader), and would execute the "goto _startup".  This
//would effective reset the application.

//To fix this situation, we should always deliberately place a
//"goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS" at address 0x08, and a
//"goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS" at address 0x18.  When the output
//hex file of this project is programmed with the bootloader, these sections do not
//get bootloaded (as they overlap the bootloader space).  If the output hex file is not
//programmed using the bootloader, then the below goto instructions do get programmed,
//and the hex file still works like normal.  The below section is only required to fix this
//scenario.
#pragma code HIGH_INTERRUPT_VECTOR = 0x08

void High_ISR(void) {
    _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
}
#pragma code LOW_INTERRUPT_VECTOR = 0x18

void Low_ISR(void) {
    _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
}
#endif	//end of "#if defined(PROGRAMMABLE_WITH_USB_HID_BOOTLOADER)||defined(PROGRAMMABLE_WITH_USB_LEGACY_CUSTOM_CLASS_BOOTLOADER)"

#pragma code


//These are your actual interrupt handling routines.
#pragma interrupt YourHighPriorityISRCode

void YourHighPriorityISRCode() {
    //Check which interrupt flag caused the interrupt.
    //Service the interrupt
    //Clear the interrupt flag
    //Etc.
#if defined(USB_INTERRUPT)
    USBDeviceTasks();
#endif

} //This return will be a "retfie fast", since this is in a #pragma interrupt section
#pragma interruptlow YourLowPriorityISRCode

void YourLowPriorityISRCode() {
    //Check which interrupt flag caused the interrupt.
    //Service the interrupt
    //Clear the interrupt flag
    //Etc.

} //This return will be a "retfie", since this is in a #pragma interruptlow section
#endif












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




static const WORD_VAL gain_max = 0xffff;
static const WORD_VAL dac_max = 0xffff;
static const WORD_VAL triggerLevelInit = 2000;

enum {
    TRIGGER_CH_1,
    TRIGGER_CH_2,
    TRIGGER_FREE_RUN
} currentTrigger;

#define LedBlink2() do { OpenCompare1(COM_INT_OFF & COM_TOGG_MATCH, 0x0000); } while (0)
#define LedBlink() { CCP1CONbits.CCP1M = 0b0010; CCPR1 = 0xffff; }
#define LedOn() { CloseCompare1(); LED = 1; }
#define LedOff()  { CloseCompare1(); LED = 0; }

#define SetTriggerLevel(level) { \
    DAC_Trigger_CS = 0; \
    DAC_Write(level, dac_max);\
    DAC_Trigger_CS = 1;\
}

#define SetGain(gainA, gainB) { \
    DAC_Gain_CS = 0; \
    DAC_Write(gainB, gainA); \
    DAC_Gain_CS = 1; \
}

#define SelectTrigger(x) { \
  currentTrigger = (x) ;\
}




void UserInit(void) {
    LATB &= ~((1 << 2) | 1 << 3 | 1 << 4);
    LATD = 0x0;
    LATE &= ~(1 << 0 | 1 << 1 | 1 << 2);

    TRISB &= ~((1 << 2) | 1 << 3 | 1 << 4);
    TRISE &= ~(1 << 0 | 1 << 1 | 1 << 2);
    TRISD = 0xff;
    TRISC &= ~(1 << 2);

    TRISA &= ~(1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);

    mADC_A0 = 0;
    mADC_A1 = 0;
    mADC_CS = 1;
    mADC_RD = 1;

    DAC_Trigger_CS = 1;
    DAC_Gain_CS = 1;
    DAC_SCLK = 0;
    DAC_DIN = 0;

    ACDC_Ch1 = 0;
    ACDC_Ch2 = 0;

#if 0
    // this makes the device dysfuntional!
   OpenTimer1(T1_OSC1EN_OFF & T1_PS_1_8 & T1_SOURCE_INT & T1_SYNC_EXT_OFF);
#else

    T1CONbits.T1OSCEN = 0;

    T1CONbits.T1CKPS = 0b11;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
#endif

    SelectTrigger(TRIGGER_FREE_RUN);
    SetTriggerLevel(triggerLevelInit);
    SetGain(gain_max, gain_max);
    LedBlink();

    //    INTCON2bits.RBPU = 0;

    //initialize the variable holding the handle for the last
    // transmission
    USBOutHandle = 0;
    USBInHandle = 0;
}//end UserInit

void DAC_Write(WORD_VAL daca, WORD_VAL dacb) {
    int i;
    for (i = 12 - 1; i >= 0; i--) {
        DAC_DIN = (dacb.Val >> i) & 1;
        DAC_SCLK = 0;
        DAC_SCLK = 1;
    }
    for (i = 12 - 1; i >= 0; i--) {
        DAC_DIN = (daca.Val >> i) & 1;
        DAC_SCLK = 0;
        DAC_SCLK = 1;
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
enum {
    STATE_NORM,
    STATE_TRANS_ONE,
    STATE_TRANS_ADC,
    STATE_TRANS_ADC_LAST = STATE_TRANS_ADC + 4
} state = STATE_NORM;


void ProcessIO(void) {
    WORD_VAL wa;
    WORD_VAL wb;

    BlinkUSBStatus();
    // User Application USB tasks
    if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1)) return;

    if (state == STATE_NORM && !HIDRxHandleBusy(USBOutHandle)) //Check if data was received from the host.
    {
        ToSendDataBuffer[0] = ReceivedDataBuffer[0] + 1;
        state = STATE_TRANS_ONE;

        switch (ReceivedDataBuffer[0]) //Look at the data the host sent, to see what kind of application specific command it sent.
        {

            case 0x10:
            {
                BYTE_VAL b = ReceivedDataBuffer[1];

                ACDC_Ch1 = b.bits.b0;
                ACDC_Ch2 = b.bits.b1;

                ToSendDataBuffer[1] = ACDC_Ch1;
                ToSendDataBuffer[2] = ACDC_Ch2;

            }
                break;

            case 0x20:

                wa.v[0] = ReceivedDataBuffer[1];
                wa.v[1] = ReceivedDataBuffer[2];

                SetTriggerLevel(wa);
                break;

            case 0x30:

                wa.v[0] = ReceivedDataBuffer[1];
                wa.v[1] = ReceivedDataBuffer[2];

                wb.v[0] = ReceivedDataBuffer[3];
                wb.v[1] = ReceivedDataBuffer[4];

                SetGain(wa, wb);
                break;

            case 0x40:
                ToSendDataBuffer[1] = ReadSingleADC();
                break;

            case 0x50:

                mADC_A0 = (ReceivedDataBuffer[1]) &1;
                mADC_A1 = (ReceivedDataBuffer[1] >> 1) &1;
                ToSendDataBuffer[1] = mADC_A0;
                ToSendDataBuffer[2] = mADC_A1;
                break;


            case 0x60:
            {

                //sample without delay


                // store acknoledge
                BYTE_VAL w = ReceivedDataBuffer[0] + 1;

                WORD timeout = 0xffff;
                int i;
                state = STATE_TRANS_ADC;

                LedBlink();

                if (currentTrigger == TRIGGER_CH_1) {
                    while (TriggerCh1 && timeout-- > 0);
                    while (!TriggerCh1 && timeout-- > 0);
                } else if (currentTrigger == TRIGGER_CH_2) {
                    while (TriggerCh2 && timeout-- > 0);
                    while (!TriggerCh2 && timeout-- > 0);
                }
                LedOff();

               // INTCONbits.GIE = 0;
                mADC_CS = 0;

                SampleNoDelaySingleChannel();

                mADC_CS = 1;
                //INTCONbits.GIE = 1;


                ToSendDataBuffer[0] = w.Val;

                LedOn();
            }
                break;


            case 0x70:
            {
                // sample with delay


                BYTE_VAL w = ReceivedDataBuffer[0] + 1;
                const BYTE del = ReceivedDataBuffer[1];
                WORD timeout = 0xffff;
                BYTE del2;
                int i;
                state = STATE_TRANS_ADC;

                INTCONbits.GIE = 0;

                LedBlink();
                if (currentTrigger == TRIGGER_CH_1) {
                    while (TriggerCh1 && timeout-- > 0);
                    while (!TriggerCh1 && timeout-- > 0);
                } else if (currentTrigger == TRIGGER_CH_2) {
                    while (TriggerCh2 && timeout-- > 0);
                    while (!TriggerCh2 && timeout-- > 0);
                }
                LedOff();

                mADC_CS = 0;
#define Sample(buffer) {\
                    for (i = 0; i < 64; i++) { \
                        mADC_RD = 0; \
                        del2 = del;\
                        while(del2--);\
                        /*  while (mADC_INT); */\
                        (buffer)[i] = PORTD; \
                        mADC_RD = 1; \
                    } \
}


                Sample(ToSendDataBuffer);
                Sample(ADC_DATA1);
                Sample(ADC_DATA2);

#undef Sample

                INTCONbits.GIE = 1;
                mADC_CS = 1;
                ToSendDataBuffer[0] = w.Val;

                LedOn();
            }
                break;

            case 0x80:
                SelectTrigger(ReceivedDataBuffer[1]);
                break;

            case 0x90:
                //ping
                break;

            case 0x81: //Get push button state
                ToSendDataBuffer[1] = 0x00;
                break;
            default:
                ToSendDataBuffer[0] = 0xff;
        }
    }

    switch (state) {
        case STATE_NORM:
            break;
        case STATE_TRANS_ONE:
        {
            if (!HIDTxHandleBusy(USBInHandle)) {
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = STATE_NORM;
            }
            //Re-arm the OUT endpoint for the next packet
            USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedDataBuffer, RX_SIZE);

            break;
        }
        case STATE_TRANS_ADC:
            if (!HIDTxHandleBusy(USBInHandle)) {
                USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                state = state + 2;
            }
            break;
            /*
                    case STATE_TRANS_ADC + 1:
                        if (!HIDTxHandleBusy(USBInHandle)) {
                            //memcpy((BYTE*) & ToSendDataBuffer[0], (void*) &ReceivedDataBuffer[0], 64);
                            //USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ToSendDataBuffer[0], 64);
                            USBInHandle = HIDTxPacket(HID_EP, (BYTE*) & ReceivedDataBuffer[0], 64);
                            state = state + 1;
                        }
                        break;
             */
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
                state = STATE_NORM;
            }
            //Re-arm the OUT endpoint for the next packet
            USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedDataBuffer, RX_SIZE);
            break;
    }

}//end ProcessIO


BYTE ReadSingleADC(void) {
    BYTE w;
    mADC_CS = mADC_RD = 0;
    //while (PORTBbits.RB1);
    w = PORTD;
    mADC_CS = mADC_RD = 1;
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

    BYTE b;
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.

    if (USBSuspendControl == 1) {
        b = 9;
    } else {
        if (USBDeviceState == DETACHED_STATE) {
            b = 1;
        } else if (USBDeviceState == ATTACHED_STATE) {
            b = 2;
        } else if (USBDeviceState == POWERED_STATE) {
            b = 3;
        } else if (USBDeviceState == DEFAULT_STATE) {
            b = 4;
        } else if (USBDeviceState == ADDRESS_STATE) {
            b = 5;
        } else if (USBDeviceState == CONFIGURED_STATE) {
            b = 6;
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
    USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedDataBuffer, RX_SIZE);
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
