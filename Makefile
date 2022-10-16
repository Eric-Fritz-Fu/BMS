.PHONY: clean lint

database.o: database.c
	gcc database.c -c

common.o: common.c
	gcc common.c -c

clean:
	rm -rf database.o
	rm -rf common.o

lint:
	gcc -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wconversion *.c -o tmp
	rm -rf tmp
