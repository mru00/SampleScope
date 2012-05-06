#include Common_xx.s


    code


LedInit
    global LedInit
    bsf LATC, 2
    bcf TRISC, 2
    movlw 0x00
    movwf CCP1CON
    return

LedBlink
    global LedBlink
    LedBlink_i
    return

LedOn
    global LedOn
    LedOn_i
    return

LedOff
    global LedOff
    LedOff_i
    return



    end