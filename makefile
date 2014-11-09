CC = notSet
EXES = nothing
FILE_EXISTS=$(shell [ -e /usr/lib64/openmpi/bin/mpicc ] && echo 1 || echo 0 )
ifeq ($(FILE_EXISTS), 1)
	CC = mpicc
	EXES = jacobi-dept
else    
        CC = cc
        EXES = jacobi-cray
endif

$(EXES): jac1.c
	$(CC) -o $(EXES) jac1.c -O3
clean:
	rm -f $(EXES)
