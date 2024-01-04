qsort: src/main.c src/intlib.c
	gcc -I./include -c -o src/intlib.o src/intlib.c
	ar rsv lib/libinteger.a src/intlib.o
	gcc -o bin/qsort -I./include src/main.c -L./lib -linteger 
