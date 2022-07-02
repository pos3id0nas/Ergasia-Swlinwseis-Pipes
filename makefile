CC=gcc
CFLAGS=-Wall -g
OBJ = ergasia2pipes

all: Pm Pc

Pm: Pm.c
	$(CC) $(CFLAGS) -o $(OBJ) Pm.c

Pc: Pc.c
	$(CC) $(CFLAGS) -o Pc Pc.c -lm

#make clean
clean:
	rm -rf $(OBJ) Pc
