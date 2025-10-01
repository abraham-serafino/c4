CC = gcc
COMPILE = $(CC) $(CFLAGS) -c $^ -o $@
LINK = $(CC) $(CFLAGS) $^ -o $@
CFLAGS = -std=c23
DELETE_CMD = rm -f

list-test:	Exception.o strings.o ref.o ArrayList.o ArrayList.test.o
	$(LINK)

ref-test: Exception.o strings.o ref.o ref.test.o
	$(LINK)

Exception.o: Exception.c
	$(COMPILE)

ref.o: ref.c
	$(COMPILE)

ref.test.o: ref.test.c
	$(COMPILE)

ArrayList.o: ArrayList.c
	$(COMPILE)

ArrayList.test.o: ArrayList.test.c
	$(COMPILE)

strings.o: strings.c
	$(COMPILE)

.PHONY: clean

clean:
	$(DELETE_CMD) errlist *.o list-test ref-test

