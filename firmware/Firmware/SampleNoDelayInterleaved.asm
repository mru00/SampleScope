
#include Common_xx.s

    extern ToSendDataBuffer
    extern ADC_DATA1
    extern ADC_DATA2



    udata

cnt RES 1






    code

; LATE.1 (ADC_A1) will never be set - only switch ADC_A0 (ch1/ch2)
ch1 macro
    bcf LATE,0
    endm

ch2 macro
    bsf LATE,0
    endm


sample macro nr, b

    movlw D'32'
    movwf cnt

    lfsr FSR0, b

loop_sample#v(nr)

    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf POSTINC0           ; 1
    ch1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1

    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf POSTINC0    ; 1
    ch2
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1

    decfsz cnt,1
    goto loop_sample#v(nr)

    endm






SampleNoDelayInterleaved
    global SampleNoDelayInterleaved

    ; channel select; both clear -> ch1
    bcf LATE,0
    bcf LATE,1

    ; prepare first sample
    CLEAR_RD
    nop
    nop
    nop
    nop

    ; sample 3x64
    sample 0, ToSendDataBuffer
    sample 1, ADC_DATA1
    sample 2, ADC_DATA2

    ; stop adc
    SET_RD

    return

    end
