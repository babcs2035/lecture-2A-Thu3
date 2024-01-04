CC = gcc
TARGET = calc
OBJS = main.o vector.o matrix.o

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm *.o *~ a.out
