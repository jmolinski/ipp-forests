CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS =

.PHONY: all clean

all: forests

forests: main.o input_interface.o output_interface.o bst.o commands.o
	$(CC) $(LDFLAGS) -o $@ $^

input_interface.o: input_interface.c input_interface.h safe_malloc.h
	$(CC) $(CFLAGS) -c $<

bst.o: bst.c bst.h safe_malloc.h input_interface.h
	$(CC) $(CFLAGS) -c $<

output_interface.o: output_interface.c output_interface.h
	$(CC) $(CFLAGS) -c $<

commands.o: commands.c commands.h input_interface.h bst.h output_interface.h safe_malloc.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c commands.h input_interface.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o forests

