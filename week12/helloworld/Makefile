helloworld: src/helloworld.o src/calc_magicnumber.o
	gcc -o bin/helloworld src/helloworld.c
	gcc -o bin/calc_magicnumber src/calc_magicnumber.c

.PHONY: clean
clean:
	rm -f src/*.o lib/*.a bin/*
