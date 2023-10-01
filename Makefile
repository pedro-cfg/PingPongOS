CC=gcc
CFLAGS=
DEPS=*.h
OBJ=ppos-core-aux.o pingpong-preempcao.o -lm
LIB=libppos_static.a

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

ppos-test: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LIB)
	rm *.o