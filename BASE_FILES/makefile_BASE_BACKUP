#############################################################
##
##  Lab 5      : Thread Scheduling
##	Programmer : Drew Rife
##	Course 	   : CMPE 320
##  Section    : 2 (11-12:50pm)
##	Instructor : S. Lee
##
#############################################################
CC = gcc
CFLAGS = -g
OBJ = ThreadScheduler.o
LIBS ?= -lpthread -lrt

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

scheduling: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJ) scheduling
