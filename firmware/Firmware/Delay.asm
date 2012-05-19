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
