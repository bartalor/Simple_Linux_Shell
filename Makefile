all: shell.c
	gcc -Werror shell.c -o shell

.phony: clean
clean:
	rm -f shell