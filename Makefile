all: nagatoro

nagatoro: nagatoro.c
	$(CC) -O2 -Wall -Werror -o $@ $<

clean:
	rm -f nagatoro

.PHONY: all clean

