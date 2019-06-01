CC=gcc
export VERSION = $(shell bash ../scripts/version.sh) # The single, trailing blank is essential
export DATE    = $(shell bash ../scripts/date.sh) #    The single, trailing blank is essential
CFLAGS         = -DVERSION=\"$(VERSION)\" -DDATE=\"$(DATE)\" -O3 -Wall -Wshadow -pedantic -std=gnu99 -g -I../common/ -L../common/ #-pg

SRCFILES  = new2view.c interface.c rooted.c unrooted.c latex.c
OBJFILES  = new2view.o interface.o rooted.o unrooted.o latex.o
LIBS      = -lcommon -lm
EXECFILE  = new2view

.PHONY : all
all : $(EXECFILE)
$(EXECFILE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)
interface.o: interface.h

clean:
	rm -f *.o *~
test:
	bash ../scripts/new2view.sh 
