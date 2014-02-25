all:
	gcc -g -lGL -lglut -lfreeimage -o lytrogl opengl.c main.c
