all:
	gcc -Wall -o interpretador interpretador.c
	gcc -Wall -o escalonador escalonador.c

interpretar: interpretador.c
	gcc -Wall -o interpretador interpretador.c
	./interpretador

escalonar: escalonador.c
	gcc -Wall -o escalonador escalonador.c queue.c
	./escalonador