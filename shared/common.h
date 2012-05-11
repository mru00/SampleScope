/**
  Definitions, constants and data structures shared by UI and firmware.
  */

#ifndef COMMON_H
#define COMMON_H


#ifdef __cplusplus
extern "C" {
#endif

#define VENDOR_ID    0xdead
#define PRODUCT_ID   0xbeef

// recreate typedefs in alignment with MPLAB
#ifdef OSCI_GUI
#  include <stdint.h>
typedef uint8_t BYTE;
typedef uint16_t WORD;
#  define __PACKED
#  define __PACKED2 __attribute__((packed))
#else
#  define __PACKED2
#endif




#ifdef __cplusplus
#define GET_CONST(x) DeviceConstants::x
class DeviceConstants {
public:
#else
#define GET_CONST(x) x
#endif

typedef enum {
    OP_SET_CONFIG = 0x00,
            OP_SAMPLE_DUMMY_TRI = 0x10,
            OP_SAMPLE_DUMMY_ZERO = 0x20,
            OP_SAMPLE_DUMMY_MID = 0x22,
    OP_READ_SINGLE = 0x40,
    OP_SAMPLE_SINGLE = 0x60,
    OP_SAMPLE_INTERLEAVED = 0x30,
    OP_PING = 0x90
} opcodes_t;

typedef enum{
    TRIGGER_RISING,
    TRIGGER_FALLING
} TriggerMode_t;

typedef enum{
    ADC_ch1 = 0,
    ADC_ch2 = 1,
    ADC_triggerLevel = 2
} Channel_t;

typedef enum {
    TRIGGER_CH_1 = 0,
    TRIGGER_CH_2 = 1,
    Trigger_Free = 2
}TriggerSource_t ;

typedef enum {
    ACDC_AC = 1,
    ACDC_DC = 0
} ACDC_t;

enum VdivValues_t {
    Vdiv_10,
    Vdiv_5,
    Vdiv_2,
    Vdiv_1,
    Vdiv_05,
    Vdiv_02,
    Vdiv_01,
    Vdiv_005,
    Vdiv_LAST = Vdiv_005
};

enum TdivValues_t {
    Tdiv_max, // = 0.0128
    Tdiv_01, Tdiv_03,
    Tdiv_1, Tdiv_3,
    Tdiv_10, Tdiv_30,
    Tdiv_100, Tdiv_300,
    Tdiv_1000, Tdiv_3000,
    Tdiv_LAST = Tdiv_3000
};

typedef enum {
   TestSignal_Off,
    TestSignal_Tri,
    TestSignal_Zero,
    TestSignal_Mid,
    TestSignal_LAST = TestSignal_Mid
} TestSignal_t;

typedef struct __PACKED {

    /* MPLAB won't have bitfields in WORDS, no 12 bit for gain, triggerLevel */
    WORD version;
    WORD triggerLevel;
    WORD gainCh1;
    WORD gainCh2;
    WORD sampleDelay;

    Channel_t inputChannel : 3;
    ACDC_t acdcCh1: 1;
    ACDC_t acdcCh2: 1;
    TriggerSource_t triggerChannel : 2;
    TriggerMode_t triggerMode : 1;

} __PACKED2 osci_config_t;

#ifdef __cplusplus
};
#endif

#define InitOsciConfig(instance) { \
    instance.version = 0xdead; \
    instance.inputChannel = GET_CONST(ADC_ch1);\
    instance.acdcCh1 = GET_CONST(ACDC_DC); \
    instance.acdcCh2 = GET_CONST(ACDC_DC); \
    instance.triggerLevel = 2000; \
    instance.triggerChannel = GET_CONST(Trigger_Free); \
    instance.triggerMode = GET_CONST(TRIGGER_RISING); \
    instance.gainCh1 = 0xffff; \
    instance.gainCh2 = 0xffff; \
    }

#ifdef __cplusplus
}
#endif


#endif // defined COMMON_H

