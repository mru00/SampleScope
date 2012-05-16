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
    OP_GET_INFO = 0x02,
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
    Tdiv_100,
    Tdiv_LAST = Tdiv_100
};

typedef enum {
   TestSignal_Off,
    TestSignal_Tri,
    TestSignal_Tri_2,
    TestSignal_Zero,
    TestSignal_Mid,
    TestSignal_LAST = TestSignal_Mid
} TestSignal_t;

typedef struct __PACKED {
    WORD version;
    WORD sizeInfo;
    WORD sizeConfig;
    WORD bufferCount;
    WORD bufferSize;
} __PACKED2 device_info_t;

typedef struct __PACKED {

    /* MPLAB won't have bitfields in WORDS, no 12 bit for gain, triggerLevel */
    WORD triggerLevel;
    WORD gainCh1;
    WORD gainCh2;
    WORD sampleDelay;

    ACDC_t acdcCh1: 1;
    ACDC_t acdcCh2: 1;
    TriggerSource_t triggerChannel : 2;
    TriggerMode_t triggerMode : 1;

} __PACKED2 osci_config_t;


typedef struct __PACKED {
    Channel_t inputChannel : 3;
} __PACKED2 READ_SINGLE_ARGS;

typedef struct __PACKED {
    Channel_t inputChannel : 3;
    TestSignal_t testSignal : 4;
} __PACKED2 SAMPLE_SINGLE_ARGS;

typedef struct __PACKED {
    BYTE placeholder;
} __PACKED2 SAMPLE_INTERLEAVED_ARGS;

typedef struct __PACKED {
    opcodes_t opcode : 8;
    union {
        osci_config_t set_config_args;
        READ_SINGLE_ARGS read_single_args;
        SAMPLE_SINGLE_ARGS sample_single_args;
        SAMPLE_INTERLEAVED_ARGS sample_interleaved_args;
    } args;
} __PACKED2 opcode_decoder_t;

typedef struct __PACKED {
    BYTE ack;
    union {
        device_info_t get_info_args;
    } args;
} __PACKED2 result_encoder_t;


#ifdef OSCI_GUI
#  define MAKE_OPCODE_ENCODER(var_, loc_) DeviceConstants::opcode_decoder_t* var_ = (DeviceConstants::opcode_decoder_t*)(loc_)
#  define MAKE_RESULT_DECODER(var_, loc_) DeviceConstants::result_encoder_t* var_ = (DeviceConstants::result_encoder_t*)(loc_)
#else
#  define MAKE_OPCODE_ENCODER(var_, loc_) opcode_decoder_t* var_ = (opcode_decoer_t*)(loc_)
#  define MAKE_RESULT_DECODER(var_, loc_) result_encoder_t* var_ = (result_encoder_t*)(loc_)
#endif

#ifdef __cplusplus
};
#endif

#define InitOsciConfig(instance) { \
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

