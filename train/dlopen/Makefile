CC=gcc
CFLAGS = -Werror

all:dl

dl: dlopen.c libdl.so
	gcc $(CFLAGS) -o $@ $< -ldl -rdynamic
libdl.so: so.c so.h
	gcc $(CFLAGS) -fPIC -shared -o $@ $^

PHONY:clean
clean:
	rm -f dl libdl.so