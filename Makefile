CC := gcc

SRCS=$(wildcard *.c)
BINS=$(patsubst %.c,%,$(SRCS))

all: $(BINS)
clean:
	find . -perm +100 -type f -delete
install:
	cp most-similar /usr/local/bin/most-similar
