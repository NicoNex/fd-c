CC=gcc
CFLAGS=-O2


fd:
	$(CC) $(CFLAGS) -o fd args.c main.c

.PHONY: clean

clean:
	rm fd
