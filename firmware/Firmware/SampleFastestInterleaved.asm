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


    global SampleFastestInterleaved


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

    movlw D'128'
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

    decfsz cnt,f
    goto loop_sample#v(nr)

    endm



SampleFastestInterleaved

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
