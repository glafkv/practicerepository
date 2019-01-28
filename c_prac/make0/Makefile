hello: hello.o
	gcc -o hello hello.o

hello.o: hello.c
	gcc -c -g hello.c

.PHONY: clean
clean:
	/bin/rm -rf *.o *~ hello
