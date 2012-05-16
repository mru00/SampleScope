#include Common.s


    code


LedInit
    global LedInit
    bsf LATC, 2
    bcf TRISC, 2
    movlf 0x00, CCP1CON

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

