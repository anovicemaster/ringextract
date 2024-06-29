
src = $(wildcard *.c)
obj = $(src:.c=.o)
lib = libringextract.a

CC = gcc
CFLAGS = -std=c17 -O2 -I$(vend)/include
LDFLAGS = -L$(vend)/lib -lcurl
ARFLAGS = rcs

$(lib): $(obj)
	$(AR) $(ARFLAGS) $@ $^


.PHONY: clean
clean:
	rm $(lib) $(obj)
