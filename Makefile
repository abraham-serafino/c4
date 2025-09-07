CC = gcc
COMPILE = $(CC) $(CFLAGS) -c $^ -o $@
LINK = $(CC) $(CFLAGS) $^ -o $@
CFLAGS = -std=c23
DELETE_CMD = rm -f

store-test:	Exception.o strings.o Store.o Store.test.o
	$(LINK)

ref-test: Exception.o strings.o ref.o ref.test.o
	$(LINK)

Exception.o: Exception.c
	$(COMPILE)

ref.o: ref.c
	$(COMPILE)

ref.test.o: ref.test.c
	$(COMPILE)

Store.o: Store.c
	$(COMPILE)

Store.test.o: Store.test.c
	$(COMPILE)

strings.o: strings.c
	$(COMPILE)

.PHONY: clean

clean:
	$(DELETE_CMD) *.o store-test ref-test
