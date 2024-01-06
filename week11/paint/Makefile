paint: src/paintapp.c src/canvas.c src/command.c src/display.c
	gcc -I./include -c -o src/canvas.o src/canvas.c
	gcc -I./include -c -o src/command.o src/command.c
	gcc -I./include -c -o src/display.o src/display.c
	ar rsv lib/libpaintapp.a src/canvas.o src/command.o src/display.o
	gcc -o bin/paintapp -I./include src/paintapp.c -L./lib -lpaintapp

.PHONY: clean
clean:
	rm -f src/*.o lib/*.a bin/*
