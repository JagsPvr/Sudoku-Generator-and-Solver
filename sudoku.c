#include "sudoku.h"
#include<math.h>
#include<stdio.h>
int sudokuSolver(sudoku A, int row, int col) {
	if (row == A -> type)
		return 1;
	if (A -> puzzle[row][col]) {
		if (col == (A -> type) - 1) {
			if(sudokuSolver(A, row + 1, 0))
				return 1;
		}
		else {
			if(sudokuSolver(A, row, col + 1))
				return 1;
		}
	}
	for (int num = 1; num <= A -> type ; num++) {
		if(isvalid(num, A, row, col)) {
			A -> puzzle[row][col] = num;
			if (col == (A -> type) - 1) {
				if(sudokuSolver(A, row + 1, 0))
					return 1;
			}
			else {
				if(sudokuSolver(A, row, col + 1))
					return 1;
			}
			//if failed to find num
			A -> puzzle[row][col] = 0;
		}
	}
	return 0;
}
int countzero(sudoku A) {
	int count = 0;
	for(int row = 0; row < (A->type-1); row++)
		for(int col = 0; col < (A->type-1); col++)
			if(A->puzzle[row][col] == 0)
				count++;
	return count;
}
int inrow(sudoku A, int row, int num) {
	for (int col = 0; col < A->type; col++) {
		if (A -> puzzle[row][col] == num)
			return 1;
	}
	return 0;
}

int incol(sudoku A, int col, int num) {
	for (int row = 0; row < A->type; row++) {
		if (A -> puzzle[row][col] == num)
			return 1;
	}
	return 0;
}
int inbox(sudoku A, int row, int col, int num) {
	for(int i = 0; i < sqrt(A->type); i++)
		for (int j = 0; j < sqrt(A->type); j++)	
			if(A -> puzzle[i + row][j + col] == num)
				return 1;
	return 0;
}
int isvalid(int num, sudoku A, int row, int col) {
	return !inrow(A,row,num) && !incol(A,col,num) && !inbox(A,row-row%3,col-col%3,num);
}
void print(sudoku A) {
	for(int row = 0; row < A->type; row++) {
		for (int col = 0; col < A->type; col++)
			printf("%3d",A->puzzle[row][col]);
		puts("");
	}
