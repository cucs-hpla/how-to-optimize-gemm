OLD  := MMult_4x4_17
NEW  := MMult_4x4_16p
#
# sample makefile
#

CC         := gcc
LINKER     := $(CC)
CFLAGS     := -O3 -Wall -msse3 -march=native -g -pg
LDFLAGS    := -lm -lblas

UTIL       := copy_matrix.o \
              compare_matrices.o \
              random_matrix.o \
              dclock.o \
              REF_MMult.o \
              print_matrix.o

TEST_OBJS  := test_MMult.o ../$(NEW).o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all:
	make clean;
	make test_MMult.x

test_MMult.x: $(TEST_OBJS) $(UTIL) parameters.h
	$(LINKER) $(TEST_OBJS) $(UTIL) $(LDFLAGS) \
        $(BLAS_LIB) -o $(TEST_BIN) $@

run:
	make all
	echo "version = '$(NEW)';" > output_$(NEW).m
	OMP_NUM_THREADS=1 ./test_MMult.x | tee -a output_$(NEW).m
	cp output_$(OLD).m output_old.m
	cp output_$(NEW).m output_new.m

clean:
	rm -f *.o *~ core *.x ../*.o

cleanall:
	rm -f *.o *~ core *.x output*.m *.eps *.png
