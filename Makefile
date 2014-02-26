all:
	gcc -g -lGL -lglut -lfreeimage `pkg-config --cflags --libs gtk+-2.0` -o lytrogl opengl.c main.c
