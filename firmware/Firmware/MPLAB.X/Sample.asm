#include P18F4550.INC


sample_one macro l
    
    movf PORTD,W      ; 1
    bsf LATB,RB2      ; 1
    bcf LATB,RB2      ; 1
    movwf l           ; 1 cycle
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1

                      ;  -> 8 cycles
                      ; 48MhZ / 4 clocks per cycle = 12 M cycles / sec
                      ; 12 M cycles per sec / 8 cycles = 1.5 Msps
                      ; 1.5 MSps = 666 ns per sample
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

SampleNoDelay
    global SampleNoDelay

    ; prepare first sample
    bcf LATB,RB2
    nop
    nop
    nop
    nop

    ; sample 3x64
    sample ToSendDataBuffer
    sample ADC_DATA1
    sample ADC_DATA2

    ; stop adc
    bsf LATB,RB2

    movlb 0
    return

    end
