# Makefile for lab10

project: project.c
	gcc project.c gfx.o -lX11 -o projects

clean:
	rm projects

