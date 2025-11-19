build:
	gcc -o tema3 FunctionsGraph.c main.c -g -std=c99 -std=gnu99 -O0

run:
	./tema3

clean:
	rm tema3

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema3