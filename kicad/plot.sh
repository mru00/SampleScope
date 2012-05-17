#! /bin/bash -xe


# first, plot from eeschema, 
# second, print from pcbnew

for ps in *.ps; do
  ps2pdf "$ps"
done

ps2pdf pcb.ps 
mv pcb.pdf ../doc/pcb.pdf

pdftk osci-*.pdf cat output ../doc/schematics.pdf
rm -f *.pdf 

