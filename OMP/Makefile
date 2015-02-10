CC=icc -std=c99 -w -fopenmp
CFLAGS=-I include/
OBJS=prime
SRC=src/
OBJS= rnd.o ziggurat.o lib.o


all:	bucketSort_OMP

bucketSort_OMP:	$(OBJS) $(SRC)bucketSort_OMP.c
	$(CC) -o bucketSort_OMP $(CFLAGS) $(OBJS) $(SRC)bucketSort_OMP.c -lm
	rm -rf $(OBJS)

rnd.o:	$(SRC)rnd.c
	$(CC) $(CFLAGS) -c $(SRC)rnd.c
	
ziggurat.o: $(SRC)ziggurat.c
	$(CC) $(CFLAGS) -c $(SRC)ziggurat.c

lib.o: $(SRC)lib.c
	$(CC) $(CFLAGS) -c $(SRC)lib.c

clean:
	rm -rf bucketSort_OMP
	rm -rf $(OBJS)
