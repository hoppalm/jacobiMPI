if [ ! -f /usr/lib64/openmpi/bin/mpicc ]; then
echo "File not found!"
else
echo "Found"
fi

CC  = mpicc

EXES = jac1

all: jac1

jac1: jac1.c
	$(CC) -o jac1 jac1.c -O3

clean:
	rm -f $(EXES)