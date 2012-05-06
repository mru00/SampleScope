
#include Common_xx.s

sample_one macro l
    
    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf l           ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1

                      ;  -> 8 cycles
                      ; 48MhZ / 4 clocks per cycle = 12 M cycles / sec
                      ; 12 M cycles per sec / 8 cycles = 1.5 Msps
                      ; 1.5 MSps = 666 ns per sample = 6.66e-7

                      ; validated: a 50kHz sine took 30 samples
                      ; for one period, 2pi.
                      ; 1/50e3 = 2e-5, 2e-5/30 = 6.66e-7
    endm

sample macro b
    banksel b
    local i
i = 0
    while i < D'64'
    sample_one b+i
i += 1
    endw
    endm

    idata

    code

    extern ToSendDataBuffer
    extern ADC_DATA1
    extern ADC_DATA2

SampleNoDelaySingleChannel
    global SampleNoDelaySingleChannel

    ; prepare first sample
    CLEAR_RD
    nop
    nop
    nop
    nop

    ; sample 3x64
    sample ToSendDataBuffer
    sample ADC_DATA1
    sample ADC_DATA2

    ; stop adc
    SET_RD

    movlb 0
    return

    end
