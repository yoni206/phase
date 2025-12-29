
TARGETS=count

all: $(TARGETS)

clean: 
	rm -f $(TARGETS) *.o

count: count.c
	gcc -o count -O3 count.c
