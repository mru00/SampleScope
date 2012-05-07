
#include Common_xx.s



    global Delay
    global DelayLSB
    global DelayMSB


    udata

DelayLSB RES 1
DelayMSB RES 1




    code


Delay
    incf DelayLSB,F
    incf DelayMSB,F
loop
    ;nop
    ;nop
    decfsz DelayLSB,F
    goto loop
    decfsz DelayMSB,F
    goto loop

done
    return

    end
