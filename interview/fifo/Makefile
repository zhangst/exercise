CC=gcc

all:fifo
fifo:fifo_test.c fifo.c fifo.h ../stack_arr.c ../stack_arr.h
	$(CC) -o $@ $^

PHONY:clean
clean:
	rm -f fifo