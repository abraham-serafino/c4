CC = gcc
COMPILE = $(CC) $(CFLAGS) -c $^ -o $@
LINK = $(CC) $(CFLAGS) $^ -o $@
CFLAGS = -std=c23
DELETE_CMD = rm -f

store-test:	Exception.o Store.o Store.test.o
	$(LINK)

Exception.o: Exception.c
	$(COMPILE)

Store.o: Store.c
	$(COMPILE)

Store.test.o: Store.test.c
	$(COMPILE)

.PHONY: clean

clean:
	$(DELETE_CMD) Exception.o Store.o Store.test.o store-test
