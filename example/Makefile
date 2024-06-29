
src = $(wildcard *.c)
bin = example

CC = gcc
CFLAGS = -std=c17 -I$(vend)/include -I../src
LDFLAGS = -L$(vend)/lib -L../src -lcurl -lringextract

$(bin): $(src)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm $(bin)
