usb:

LIBUSB_CXX = $(shell pkg-config --cflags libusb-1.0)
LIBUSB_LD = $(shell pkg-config --libs libusb-1.0)

CXXFLAGS = -I/usr/local ${LIBUSB_CXX} -DLINUX
CFLAGS = ${CXXFLAGS}
LDADD = /home/mru/opt/libusb-1.0.9/libusb/libusb-1.0.la
LDFLAGS = ${LIBUSB_LD} ${LDADD}
LIBS = ${LDFLAGS}

usb: usb.c
	${CC} ${CFLAGS} -o usb usb.c -pthread -LHIDAPI/linux -lHIDAPI -lusb-1.0 
