CC=gcc
CFLAGS=-O3 -Wall
LDFLAGS=-lpci

radeonvolt: radeonvolt.o i2c.o vt1165.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

radeonvolt.o: radeonvolt.c
	${CC} ${CFLAGS} -c $<

i2c.o: i2c.c
	${CC} ${CFLAGS} -c $<

vt1165.o: vt1165.c
	${CC} ${CFLAGS} -c $<

.PHONY: clean
clean:
	rm -f radeonvolt *.o
