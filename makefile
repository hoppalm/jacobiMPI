CC  = mpicc

EXES = jac1

all: jac1

jac1: jac1.c
	$(CC) -o jac1 jac1.c -O3

clean:
	rm -f $(EXES)


