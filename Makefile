CC=gcc
CFLAGS= -lrt
DEPS=*.h
OBJ=ppos-core-aux.o disk.o ppos_disk.o pingpong-disco2.o
LIB=libppos_static.a

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

ppos-test: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIB)
	rm *.o