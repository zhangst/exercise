.phony:all clean

all:test balance_symbol

test:test.c list.c
	gcc -O2 -Wall -o $@ $^
balance_symbol:balance_symbol.c stack_list.c
	gcc -O2 -Wall -o $@ $^

clean:
	rm -f test balance_symbol
