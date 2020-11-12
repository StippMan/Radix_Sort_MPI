EXECUTABLES = p_radix_sort_full_time p_radix_sort_partial_time
DEBUG_FLAGS = -O3

all: $(EXECUTABLES)


p_radix_sort_partial_time : p_radix_sort_partial_time.c
	mpicc -std=gnu99 $(DEBUG_FLAGS) -o$@ $^ -lrt

p_radix_sort_full_time : p_radix_sort_full_time.c
	mpicc -std=gnu99 $(DEBUG_FLAGS) -o$@ $^ -lrt

%.o : %.c %.h
	mpicc $(DEBUG_FLAGS) -c -std=gnu99 $<

clean:
	rm -f $(EXECUTABLES) *.o mpe-*
