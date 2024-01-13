#################################################################################

DEBUG  = -g
ARFLAGS = rcs
CFLAGS  = $(DEBUG) -Wall $(INCLUDE)
LDLIBS  = -ljansson -lpthread

###############################################################################

SRC     =	logapp.c
OBJ     =	$(SRC:.c=.o)
BINS    =	$(SRC:.c=)

##############################################################################


logapp:	logapp.o
	$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LDLIBS) -static

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ) *~ core tags $(BINS)

tags:	$(SRC)
	ctags $(SRC)

depend:
	makedepend -Y $(SRC)
