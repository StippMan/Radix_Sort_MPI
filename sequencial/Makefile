EXECUTABLES =  radix_seq 

all: $(EXECUTABLES)

radix_seq: radix_seq.c
	gcc -std=gnu99 $(DEBUG_FLAGS) -o$@ $^ -lrt

%.o : %.c %.h
	gcc $(DEBUG_FLAGS) -c -std=gnu99 $<

clean:
	rm -f $(EXECUTABLES) *.o mpe-*

