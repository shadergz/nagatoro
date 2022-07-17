all: nagatoro

nagatoro: nagatoro.c
	$(CC) -O2 -Wall -Werror -g -o $@ $<

clean:
	rm -f nagatoro

.PHONY: all clean

