################################################################
##
## Project 2   : CPU Scheduling
## Programmers : Drew Rife & Alec Waddelow
## Course      : CMPE 320
## Section     : 2 (11-12:50pm)
## Instructor  : S. Lee
##
################################################################
CC = gcc
CFLAGS = -g 
OBJ = EDF_Scheduler.o 
LIBS ?= -lpthread -lrt

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

scheduler: $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJ) scheduler