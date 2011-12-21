all:vm_test

vm_test:test.c malloc.c memlib.c memlib.h mm.h
	gcc -o $@ $^ -Wall

.PHONY:clean test
clean:
	rm -f vm_test

test:
	./vm_test