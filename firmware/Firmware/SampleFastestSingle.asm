;SampleScope: A humble USB oscilloscope
;Copyright (C) 2012 mru
;
;This program is free software: you can redistribute it and/or modify
;it under the terms of the GNU General Public License as published by
;the Free Software Foundation, either version 3 of the License, or
;(at your option) any later version.

;This program is distributed in the hope that it will be useful,
;but WITHOUT ANY WARRANTY; without even the implied warranty of
;MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;GNU General Public License for more details.

;You should have received a copy of the GNU General Public License
;along with this program.  If not, see <http://www.gnu.org/licenses/>.






#include Common.s

;    extern ToSendDataBuffer
;    extern ADC_DATA1
;    extern ADC_DATA2

    global SampleFastestSingle


    udata
cnt RES 1


    code



                      ;  -> 8 cycles
                      ; 48MhZ / 4 clocks per cycle = 12 M cycles / sec
                      ; 12 M cycles per sec / 8 cycles = 1.5 Msps
                      ; 1.5 MSps = 666 ns per sample = 6.66e-7

                      ; validated: a 50kHz sine took 30 samples
                      ; for one period, 2pi.
                      ; 1/50e3 = 2e-5, 2e-5/30 = 6.66e-7


sample macro nr, b


    banksel cnt
    movlw D'0'
    movwf cnt

    ;banksel b
    lfsr FSR0, b

loop_lab#v(nr)

    movf PORTD,W      ; 1 cycle
    SET_RD            ; 1
    CLEAR_RD          ; 1
    movwf POSTINC0           ; 1
    nop               ; 1

    ;nop

    decfsz cnt,F
    goto loop_lab#v(nr)

    endm


SampleFastestSingle

    ; prepare first sample
    CLEAR_RD
    nop
    nop
    nop
    nop

    sample4


    ; stop adc
    SET_RD

    ;movlb 0
    return


    end
