all:
	gcc -g -lGL -lglut -lfreeimage `pkg-config --cflags --libs gtkglext-1.0` -o lytrogl opengl.c main.c
