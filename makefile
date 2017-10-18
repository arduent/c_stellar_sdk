CC = gcc-6
CFLAGS = -Wno-unused-result

FAST = -pipe -Ofast -mavx2 -march=native -funroll-loops -mfpmath=both -m64
MATH = -lm

OUT  = xlm_test_0

all: $(OUT)

xlm_test_0: sds/sds.c xlm_test_0.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OUT)
