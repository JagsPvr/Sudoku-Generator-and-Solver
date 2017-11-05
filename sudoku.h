typedef struct Sudoku {
	int puzzle[25][25];
	int type;
	/*For any N*N sudoku, here 'type' denotes 'N' i.e order of sudoku*/
} Sudoku;

void init();
int verify();
void show();
void savef();
void loadfile();
void Diagonal_box(int row,int col);
int fillit(int row, int col);
int sudokuSolver(int row,int col);
int isvalid(int row, int col, int num);
int inrow(int row,int col,int num);
int incol(int row, int col, int num);
int square(int row,int col,int num);
void fill_box(int row,int col);
void helpMe();
