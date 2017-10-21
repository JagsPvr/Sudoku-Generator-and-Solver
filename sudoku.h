typedef struct node {
	int puzzle[25][25];
	int type;
	/*For any N*N sudoku, here 'type' denotes 'N' i.e order of sudoku*/
}node;
typedef node* sudoku;
int countzero(sudoku);
int sudokuSolver(sudoku,int row,int col);
int isvalid(int,sudoku,int row, int col);
int inrow(sudoku,int row,int num);
int incol(sudoku,int row,int num);
int inbox(sudoku,int row,int col,int num);
void generate(sudoku);
int iscorrect(sudoku);
void print(sudoku);