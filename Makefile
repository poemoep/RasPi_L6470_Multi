CC            = gcc
CFLAGS        = -O4 -Wall -I/usr/local/include 
LDFLAGS       = -L/usr/local/lib
LIBS          = -lwiringPi -lm
MAINOBJS      = L6470.o main.o L6470_const.o
GENOBJS       = generator.o
USERH	      = L6470_user.h
USERHEADER    = user
GENPROGRAM    = gen
GENHEADER     = generator.h
PROGRAM       = main

all:            $(PROGRAM)

$(PROGRAM):     $(MAINOBJS) $(USERH)
	$(CC) $(MAINOBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

$(GENPROGRAM):  $(GENOBJS) $(GENHEADER)
	cp $(GENHEADER) $(GENHEADER).bak
	$(CC) $(GENOBJS) $(LDFLAGS) $(LIBS) -o $(GENPROGRAM)

$(USERHEADER):  $(GENPROGRAM)
	./$(GENPROGRAM) > temp.h
	echo "#if 0" >> temp.h
	cat $(USERH) >> temp.h
	echo "#endif" >> temp.h
	rm $(USERH)
	mv temp.h $(USERH)

clean:;         rm -f *.o *~ $(PROGRAM) $(GENPROGRAM)
