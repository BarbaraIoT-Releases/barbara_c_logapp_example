#################################################################################

DEBUG  = -g
CC	= $(TARGETMACH)-gcc
LD	= $(TARGETMACH)-ld
AR	= $(TARGETMACH)-ar
ARFLAGS = rcs
INCLUDE = -Imosquitto
CFLAGS  = $(DEBUG) -Wall $(INCLUDE) --sysroot=$(SYSROOT_FOLDER)
LDFLAGS = --sysroot=$(SYSROOT_FOLDER) 
LDLIBS  = -ljansson -lpthread -lssl -lcrypto -lmosquitto

###############################################################################

SRC     =         logapp.c	

OBJ     =       $(SRC:.c=.o)

BINS    =       $(SRC:.c=)

##############################################################################


  logapp:    logapp.o 
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@
clean:
	rm -f $(OBJ) *~ core tags $(BINS)

tags:   $(SRC)
	ctags $(SRC)

depend:
	makedepend -Y $(SRC)


