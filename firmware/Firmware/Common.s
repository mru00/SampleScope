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




#include P18F4450.INC


    extern B_ADC_0
    extern B_ADC_1
    extern B_ADC_2
    extern B_ADC_3


SET_RD macro
    bsf LATB,RB2      ; 1
    endm

CLEAR_RD macro
    bcf LATB,RB2      ; 1
    endm


sample4 macro
    sample 0, B_ADC_0
    sample 1, B_ADC_1
    sample 2, B_ADC_2
    sample 3, B_ADC_3
    endm

LedBlink_i macro
    bsf CCP1CON, 1
    endm

LedOn_i macro
    bcf CCP1CON, 1
    bsf LATC, 2
    endm


LedOff_i macro
    bcf CCP1CON, 1
    bcf LATC, 2
    endm

;#define LedBlink2() { CCP1CONbits.CCP1M = 0b0010; CCPR1 = 0x8000; }
;#define LedBlink() { CCP1CONbits.CCP1M = 0b0010; CCPR1 = 0xffff; }
;#define LedOn() { CCP1CONbits.CCP1M = 0b0000; LED = 1; }
;#define LedOff()  { CCP1CONbits.CCP1M = 0b0000; LED = 0; }

