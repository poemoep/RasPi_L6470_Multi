CC            = gcc
CFLAGS        = -O4 -Wall -I/usr/local/include 
LDFLAGS       = -L/usr/local/lib
LIBS          = -lwiringPi -lm
MAINOBJS      = L6470.o main.o
USERH	      = L6470_user.h
PROGRAM       = main

all:            $(PROGRAM)

$(PROGRAM):     $(MAINOBJS) $(USERH)
	$(CC) $(MAINOBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;         rm -f *.o *~ $(PROGRAM) 