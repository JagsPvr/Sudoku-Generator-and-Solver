project: sudoku.c sudoku.h
	gcc sudoku.c -o project `pkg-config --libs --cflags gtk+-2.0` -lm
