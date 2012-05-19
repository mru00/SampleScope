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


/** INCLUDES *******************************************************/
#include "./USB/usb.h"
#include "./HardwareProfile.h"
#include "./USB/usb_function_hid.h"
#include <timers.h>
#include <compare.h>


#include "config_bits.h"


//#define RX_SIZE 64

/** VARIABLES ******************************************************/
#pragma udata USB_VARIABLES=0x480
unsigned char ReceivedData[64];
unsigned char ToSendDataBuffer[64];

#pragma idata

USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
extern void ProcessIO(void);
extern void UserInit(void);

//void USBCBSendResume(void);
static void BlinkUSBStatus(void);
extern void resetState(void);

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
    USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & ReceivedData, 64);
    resetState();
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

    resetState();
    // Nevertheless, this callback function is provided, such as
    // for debugging purposes.
}
