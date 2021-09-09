all: src/shell.c
	gcc -Werror src/shell.c -o shell

.phony: clean
clean:
	rm -f shell