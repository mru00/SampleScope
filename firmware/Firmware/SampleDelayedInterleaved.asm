
#include Common_xx.s


    extern ToSendDataBuffer
    extern ADC_DATA1
    extern ADC_DATA2

    extern DelayLSB
    extern DelayMSB
    extern Delay

    global SampleDelayedInterleaved


    udata

cnt RES 1
del_msb RES 1
del_lsb RES 1




    code


sample macro nr, b

    movlw D'32'
    movwf cnt

    lfsr FSR0, b

loop_sample#v(nr)

    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf POSTINC0           ; 1
    bcf LATE,0
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1

    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf POSTINC0    ; 1
    bsf LATE,0
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1
    nop               ; 1



    movff del_msb,DelayMSB
    movff del_lsb,DelayLSB

    call Delay

    decfsz cnt,1
    goto loop_sample#v(nr)

    endm



SampleDelayedInterleaved



    ; get parameters from stack
    MOVLW 0xff
    movff PLUSW1, del_msb
    MOVLW 0xfe
    movff PLUSW1, del_lsb



    ; channel select; both clear -> ch1
    bcf LATE,0
    bcf LATE,1

    ; prepare first sample
    CLEAR_RD
    nop
    nop
    nop
    nop

    banksel cnt

    sample4

    ; stop adc
    SET_RD

    return

    end
