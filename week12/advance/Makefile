knapsack: src/knapsack.o
	gcc -o bin/knapsack src/knapsack.c

tsp: src/tsp.o
	gcc -o bin/tsp src/tsp.c -lm

.PHONY: clean
clean:
	rm -f src/*.o lib/*.a bin/*
