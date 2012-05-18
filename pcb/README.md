pcb
===

contains the schematics & pcb layout.

see /doc/schematics.pdf and /doc/pcb.pdf for rendered versions, 
or check out kicad:

kicad schematic entry & pcb layout software.
http://iut-tice.ujf-grenoble.fr/kicad/

NB: the schematics and pcb layout are quite messy - i did not
draw them for the "end user" but rather as guide for me.

nevertheless, with some care, the device should be buildable.

analog
------

all analog paths (except some quick-fixes on my actual hardware) are routed
directly to fit on a perfboard. top and bottom layer are used, while
the top layer only has vertical connections.

digital
-------

all digital connections are simple point-to-point connections
with insolated wire (cf. http://en.wikipedia.org/wiki/Wiring_pencil)
_.

issues
------

* Analog bandwith is mostly limited by the PGA (MAX532) -> find a better solution
* Inputs are wired terribly, huge crosstalk if one channel is left unconnected -> change layout


