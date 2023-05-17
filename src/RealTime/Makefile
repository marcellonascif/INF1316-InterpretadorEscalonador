all:
	gcc -Wall -o interpretador_RT interpretador_RT.c
	gcc -Wall -o escalonador_RT escalonador_RT.c
	./interpretador_RT
	./escalonador_RT

interpretar: interpretador_RT.c
	gcc -Wall -o interpretador_RT interpretador_RT.c
	./interpretador_RT

escalonar: escalonador_RT.c
	gcc -Wall -o escalonador_RT escalonador_RT.c
	./escalonador_RT