CC=gcc
CFLAGS=
DEPS=*.h
OBJ=ppos-core-aux.o pingpong-contab-prio.o
LIB=libppos_static.a

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

ppos-test: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIB)
	rm *.o