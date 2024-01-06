mtfuji: src/mtfuji.c src/type.c src/sort.c src/optimize.c
	gcc -I./include -c -o src/type.o src/type.c
	gcc -I./include -c -o src/sort.o src/sort.c
	gcc -I./include -c -o src/optimize.o src/optimize.c
	ar rsv lib/libmtfuji.a src/type.o src/sort.o src/optimize.o
	gcc -o bin/mtfuji -I./include src/mtfuji.c -L./lib -lmtfuji -lm

.PHONY: clean
clean:
	rm -f src/*.o lib/*.a bin/*
