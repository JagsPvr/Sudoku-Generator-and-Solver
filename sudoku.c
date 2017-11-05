#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sudoku.h"
#define TOOL_X 94 // TO define x and y parameters of for most of the gtkWidgets
#define TOOL_Y 50
Sudoku sudoku;
int solution[25][25], help[25][25];
int diffi = 0, rescheck = 0, ccheck = 0;
/*rescheck to see if the functions come from resume or not and ccheck to see if funtions call is for final verification or not*/
static GtkWidget *wid[25][25];

void make_toolbar_play(GtkBox *box);
void make_toolbar(GtkBox *box);
void play(GtkWidget* widget, gpointer data);
void check(GtkWidget *window);
void build();
void build2();
void build25();
void get_element();
void easyNEW();
void easyNew();
void check();
void diff();
void Easy();
void Medium();
void Hard();
void aboutMe();
void init();
void new();
void show_question(GtkWidget *window,char *s);

/*This will show all dialog boxes*/
void show_question(GtkWidget *window,char *s) {
	GtkWidget *dialog;
  	dialog = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE,"%s",s);
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
  
}

void Diagonal_box(int row,int col) {
	time_t tt;
	int i, j;
	srand(time(&tt));
        int num;
	for (i = 0; i < sqrt(sudoku.type); i++) {
		for (j = 0; j < sqrt(sudoku.type); j++){
			do{
				num = rand() % sudoku.type + 1;
			}while (!isvalid(row + i, col + j, num));
			help[row + i ][col + j] = sudoku.puzzle[row + i][col + j] = num;
                }
        }
}
/*This will provide saving option for sudoku if partially saved i.e., for play */
void savef() {
	int i, j;
	FILE* savefile = NULL;
	/*This will open/create respective file to save the sudoku*/
	switch(sudoku.type) {
		case 4:
			savefile = fopen("save4.txt", "w");
			break;
		case 9:
			savefile = fopen("save9.txt", "w");
			break;
		case 16:
			savefile = fopen("save16.txt", "w");
			break;
		case 25:
			savefile = fopen("save25.txt", "w");
			break;
		default :
			puts("Not valid type");
			gtk_main_quit();
	}
	if(!savefile) {
		show_question(NULL,"Unable to save file...!");
		return;
	}
	/*File opened successfully*/
	if(sudoku.type < 16) {
		char SAVE[4];
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) {
				if(sudoku.puzzle[i][j]) {
					sprintf(SAVE, "%d%d%d", i, j, sudoku.puzzle[i][j]);
					fwrite(SAVE, 1, 3, savefile);
				}
			}
		}
		fwrite("*", 1, 1, savefile);
		/*Writiing '*' to the file to distinguish between editable and non editable cells*/
		for(i = 0; i < sudoku.type; i++) 
			for(j = 0; j < sudoku.type; j++)
				solution[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) {
				if(solution[i][j]) {
					sprintf(SAVE, "%d%d%d", i, j, solution[i][j]);
					fwrite(SAVE, 1, 3, savefile);
				}
			}
		}
	}
	/*Here using different conventions to save 2 digit in file file*/
	else {
		char SAVE[3];
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) {
				if(sudoku.puzzle[i][j]) {
					sprintf(SAVE,"%d", i);
					fwrite(SAVE, 1, 2, savefile);
					sprintf(SAVE, "%d", j);
					fwrite(SAVE, 1, 2, savefile);
					sprintf(SAVE, "%d", sudoku.puzzle[i][j]);
					fwrite(SAVE, 1, 2, savefile);
				}
			}
		}
		fwrite("*", 1, 1, savefile);
		for(i = 0; i < sudoku.type; i++) 
			for(j = 0; j < sudoku.type; j++)
				solution[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) {
				if(solution[i][j]) {
					sprintf(SAVE, "%d", i);
					fwrite(SAVE, 1, 2, savefile);
					sprintf(SAVE, "%d", j);
					fwrite(SAVE, 1, 2, savefile);
					sprintf(SAVE, "%d", sudoku.puzzle[i][j]);
					fwrite(SAVE, 1, 2, savefile);
				}
			}
		}
	}
	show_question(NULL, "Game Saved Successfully");
	fclose(savefile);	
}
/*Hint giving funcion*/
void helpMe() {
	int i, j, count = 0;
	char hint[3];
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	CHECK:
	if(!rescheck) {
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) 
				if(!solution[i][j] && !sudoku.puzzle[i][j]){
					sprintf(hint,"%d",help[i][j]);
					solution[i][j] = help[i][j] ;
					gtk_entry_set_text(GTK_ENTRY(wid[i][j]), hint);
					gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), FALSE);
					gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL,&color);
					count = 1;
					break;
				}
			if(count)
				break;
		}
	}
	else {
		for(i = 0; i < sudoku.type; i++) 
			for(j = 0; j < sudoku.type; j++)
				solution[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
		if(sudokuSolver(0,0)) {
			rescheck = 0;
			goto CHECK;
		}
		else
			show_question(NULL, "The sudoku is Incorrect");
	}
		
}
/*This will initialize the sudoku and will make all entries zero*/
void init() {
	int i, j;
	for(i = 0; i < sudoku.type; i++)
		for( j = 0; j < sudoku.type; j++) {
			solution[i][j] = sudoku.puzzle[i][j] = 0;	
		}
}
/*This will create new blank sudoku. If color to cells is there, then it will make it white(normal)*/
void new() {
	GdkColor color;
	int i, j;
	gdk_color_parse("white", &color);
	for(i = 0; i < sudoku.type; i++)
		for(j = 0; j < sudoku.type; j++) {
			help[i][j] = solution[i][j] = sudoku.puzzle[i][j] = 0;
			gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), TRUE);	
			gtk_entry_set_text(GTK_ENTRY(wid[i][j]), "");
			gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL,&color);
		}
}

/*This function will load already existing file if exist(Separate file for separate type ex. 4*4,9*9 etc.)*/
void loadFile() {
	rescheck = 1;
	GdkColor color;
	gdk_color_parse("skyblue",&color);
	int i, j, num;
	char SAVE[4];
	int count = 0;
	char c = 'a';
	
	GtkWidget *window, *vbox, *separator, *hbox;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "Resume");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	vbox = gtk_vbox_new(0,0);
	make_toolbar_play(GTK_BOX(vbox));
	for(i = 0; i < sudoku.type; i++) {
		hbox = gtk_hbox_new(0,0);
		for(j = 0; j < sudoku.type; j++) {
			wid[i][j] = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]), 2);
			gtk_widget_set_size_request(wid[i][j], 25, 25);
			gtk_box_pack_start(GTK_BOX(hbox), wid[i][j], 0, 0, 0);
			if((j+1) % (int)(sqrt(sudoku.type)) == 0) {
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator, 0, 0, 0);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox, 0, 0, 0);
		if((i+1) % (int)(sqrt(sudoku.type)) == 0) {
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,0);
		}
	}
	gtk_container_add(GTK_CONTAINER(window),vbox);
	FILE* savefile = NULL;
	switch(sudoku.type) {
		case 4:
			savefile = fopen("save4.txt", "r");
			break;
		case 9:
			savefile = fopen("save9.txt", "r");
			break;
		case 16:
			savefile = fopen("save16.txt", "r");
			break;
		case 25:
			savefile = fopen("save25.txt", "r");
			break;
		default :
			puts("Not valid type");
			gtk_main_quit();
	}
	new();
	if(!savefile) {
		show_question(NULL, "No saved game exists for this type");
		return;
	}
	if((feof(savefile)))
		show_question(NULL, "No saved game ");
	else{	
		if(sudoku.type < 16) {
			while((c = fgetc(savefile)) != '*') {
				i = c - '0';
				j = fgetc(savefile) - '0';
				num = ((c = fgetc(savefile)) - '0');
				help[i][j] = solution[i][j] = sudoku.puzzle[i][j] = num ;
				char arr[2] = {c,'\0'};
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), arr);
				gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), FALSE);
				gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL, &color);
			}
			while(!feof(savefile)) {
				i = (fgetc(savefile) - '0');
				if(!feof(savefile))
					j = (fgetc(savefile) - '0');
				else break;
				if(!feof(savefile))
					num = ((c = fgetc(savefile)) - '0');
				else break;
				char arr[2] = {c,'\0'};
				if(!sudoku.puzzle[i][j]) {
					solution[i][j] = num ;
					gtk_entry_set_text(GTK_ENTRY(wid[i][j]), arr);
					gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), TRUE);
				}
			}
		}
		else {
			while((c = fgetc(savefile)) != '*') {
				fseek(savefile, -1, SEEK_CUR);
				char str1[2];
				str1[0] = fgetc(savefile);
				str1[1] = fgetc(savefile);
				i = atoi(str1);
				str1[0] = fgetc(savefile);
				str1[1] = fgetc(savefile);
				j = atoi(str1);
				str1[0] = fgetc(savefile);
				str1[1] = fgetc(savefile);
				num = atoi(str1);
				help[i][j] = solution[i][j] = sudoku.puzzle[i][j] = num ;
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), str1);
				gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), FALSE);
				gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL, &color);
			}
			while(!feof(savefile)) {
				char str1[2];
				str1[0] = fgetc(savefile);
				str1[1] = fgetc(savefile);
				i = atoi(str1);
				if(!feof(savefile)) {
					str1[0] = fgetc(savefile);
					str1[1] = fgetc(savefile);
				}
				else break;
				j = atoi(str1);
				if(!feof(savefile)) {
					str1[0] = fgetc(savefile);
					str1[1] = fgetc(savefile);
				}
				else break;
				num = atoi(str1);
				solution[i][j] = num ;
				if(!solution[i][j]) {
					solution[i][j] = num ;
					gtk_entry_set_text(GTK_ENTRY(wid[i][j]), str1);
					gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), TRUE);
				}
			}
		}
	}	
	fclose(savefile);
	gtk_widget_show_all(window);
}
/*This will check if number is present in square or not*/
int square(int row, int col, int k) {
	if((k > sudoku.type) || !k)
		return 0;
	int g, h;
	int r = ceil((row+1) / sqrt(sudoku.type));
	int c = ceil((col+1) / sqrt(sudoku.type));
	for(g = (r-1)*sqrt(sudoku.type); g < ((r-1)*sqrt(sudoku.type) + sqrt(sudoku.type)); g++)
		for( h = (c-1)*sqrt(sudoku.type); h < ((c-1)*sqrt(sudoku.type) + sqrt(sudoku.type)); h++)
			if((row != g && col != h) && sudoku.puzzle[g][h] == k)
				return 0;
	return 1;
}
/*This will check if number is present in row or not*/
int inrow(int row, int colc, int num) {
	int col;
	if((num > sudoku.type) || !num)
		return 1;
	for (col = 0; col < sudoku.type; col++) {
		if (col != colc && sudoku.puzzle[row][col] == num)
			return 1;
	}
	return 0;
}
/*This will check if number is present in column or not*/
int incol(int crow, int col, int num) {
	int row;
	if((num > sudoku.type) || !num)
		return 1;
	for (row = 0; row < sudoku.type; row++) {
		if (crow != row && sudoku.puzzle[row][col] == num)
			return 1;
	}
	return 0;
}
/*This will check if number is valid at given row and column*/
int isvalid(int row,int col, int num) {
	return ((!inrow(row,col,num) && !incol(row,col,num)) && square(row,col,num));
}
/*This will make sudoku box as per difficulty level*/
void show() {
	int i, j, k;
	time_t tt;
	srand(time(&tt));
	int holes = diffi;
	for (k = 0; k < holes; k++) {	
		i = rand() % sudoku.type;
		j = rand() % sudoku.type;
		SOS:
		if(sudoku.puzzle[i][j] == 0){
			i = (i + 1) % sudoku.type;
			j = (j + 1) % sudoku.type;
			goto SOS;	
		}
		help[i][j] = sudoku.puzzle[i][j];
		sudoku.puzzle[i][j] = 0;
	}
	return;
}
/*This will verify if entered sudoku is right or not*/
int verify() {
	int i, j;
	for(i = 0; i < sudoku.type; i++) {
		for(j = 0; j < sudoku.type; j++) {
			if(ccheck) {
				if(isvalid(i, j, solution[i][j])) {
					continue;
				}
				else {
					ccheck = 0;
					return 0;
				}
			}
			else{
				if(solution[i][j]) {
					if(isvalid(i, j, solution[i][j]))
						continue;
					else {
						return 0;
					}
				}
			}
		}
	}
	if(!rescheck)
		for(i = 0; i < sudoku.type; i++) 
			for(j = 0; j < sudoku.type; j++) 
				if(solution[i][j] != 0)
					sudoku.puzzle[i][j] = solution[i][j];
	return 1;
}
/*This will fill sudoku grid*/
int fillit(int row, int col) {	
	int i,j;
	if(row < sudoku.type && col < sudoku.type) {	
		if(sudoku.puzzle[row][col] != 0) {
			if((col+1) < sudoku.type) 
				return fillit(row, col+1);
			else if((row+1) < sudoku.type) 
				return fillit(row+1, 0);
			else 
				return 1;
		}
		else { 
			for(i = 0; i < sudoku.type; ++i) {
				if(isvalid(row, col, i+1)) {	
					sudoku.puzzle[row][col] = help[row][col] = i+1;
					if((col+1)<sudoku.type) {
						if(fillit(row, col +1)) 
							return 1;
						else 
							help[row][col] = sudoku.puzzle[row][col] = 0;
					}
					else if((row+1) < sudoku.type) {
						if(fillit(row+1, 0)) 
							return 1;
						else 
							help[row][col] = sudoku.puzzle[row][col] = 0;
					}
					else 
						return 1;
				}
			}
		}
		return 0;
	}
	return 1;
}
/*This will solve sudoku. It will return 1 of solved else return 0*/
int sudokuSolver(int row, int col) {
	int num;
	static int k = 0;
	if(!k)
		if(!verify())
			return 0;
	k++;
	
	if (row == sudoku.type) {
		k = 0;
		return 1;
	}
	if (sudoku.puzzle[row][col]) {
		if (col == sudoku.type- 1) {
			if(sudokuSolver(row + 1, 0))
				return 1;
		}
		else {
			if(sudokuSolver(row, col + 1))
				return 1;
		}
	}
	else {
		for (num = 1; num <= sudoku.type ; num++) {
	
			if(isvalid(row, col,num)) {
				help[row][col] = sudoku.puzzle[row][col] = num;
				if (col == sudoku.type - 1) {
					if(sudokuSolver(row + 1, 0))
						return 1;
				}
				else {
					if(sudokuSolver(row, col + 1))
						return 1;
				}
				//if failed to find num
				help[row][col] = sudoku.puzzle[row][col] = 0;
			}
		}
	}
	return 0;
}
/*Gettin numbers entered by user and calling solve function*/
void get_element(GtkWidget * window) {
	int i, j;
	for(i = 0; i < sudoku.type; i++) 
		for(j = 0; j < sudoku.type; j++)
			solution[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
	if(sudokuSolver(0,0) == 1) {
	/*If solved then flushing on to the screen*/
		for(i = 0; i < sudoku.type; i++){
			for(j = 0; j < sudoku.type; j++){
				char c[3];
				sprintf(c, "%d", sudoku.puzzle[i][j]);
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), c);
			}
		}
	}
	else {
		show_question(window,"InValid sudoku..! Not solvable");
	}
}
/*This will create new puzzle and show it on screen */
void easyNEW() {
	int i, j;
	init();
	new();
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	if(sudoku.type != 25) {
		for (int i = 0; i < sudoku.type; i = i + sqrt(sudoku.type))
			Diagonal_box(i, i);
	}
	if(fillit(0,0))
		show();
	for(i = 0; i < sudoku.type; i++) {
		for(j = 0;j < sudoku.type; j++) {
				char c[3];
				if(sudoku.puzzle[i][j]) {
					sprintf(c,"%d", sudoku.puzzle[i][j]);
					gtk_entry_set_text(GTK_ENTRY(wid[i][j]), c);
					gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), FALSE);
					gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL,&color);
				
				}
		}
	}
}

/*This function will check if user solved it correctly or not. If solved then making all grid of same color*/
void check(GtkWidget *window) {
	int i, j;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	int ccheck = 1;
	for(i = 0; i < sudoku.type; i++) 
		for(j = 0; j < sudoku.type; j++)
			solution[i][j] = atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
	if(verify()) {
		for(i = 0; i < sudoku.type; i++) {
			for(j = 0; j < sudoku.type; j++) {
				gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL,&color);
			}
		}
		show_question(window, "Well Done..! You got it right");
	}
	else {
		show_question(window,"Your Answer is not correct");
	}
}
/*Toolbar for the play option*/
void make_toolbar_play(GtkBox *box){
	GtkWidget   *toolbar;
	GtkToolItem *bNew, *separator, *solve_btn, *save, *resume, *verify, *hint;
	
	toolbar = gtk_toolbar_new();
	gtk_widget_set_name(toolbar, "toolbar");
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	gtk_box_pack_start(box, toolbar, 0, 0, 0);
	/*Making new button*/
	bNew = gtk_menu_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(bNew), " Generate New Game");
	g_signal_connect(G_OBJECT(bNew), "clicked", G_CALLBACK(easyNEW), 0); 
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), bNew, 0);
	gtk_widget_show(GTK_WIDGET(bNew));
	
	separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 1);
	gtk_widget_show(GTK_WIDGET(separator));
	
	/*Making solve button */
	solve_btn = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	g_signal_connect(G_OBJECT(solve_btn), "clicked", G_CALLBACK(get_element), NULL);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(solve_btn), "Solve!");
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(solve_btn), "Solution");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), solve_btn, 2);
	gtk_widget_show(GTK_WIDGET(solve_btn));
        
        separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 3);
	gtk_widget_show(GTK_WIDGET(separator));
	
	/*Making save button*/
	save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, 4);
	g_signal_connect(G_OBJECT(save), "clicked", G_CALLBACK(savef), NULL);
	gtk_widget_show(GTK_WIDGET(save));
	
	separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 5);
	gtk_widget_show(GTK_WIDGET(separator));
		
	verify = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(verify), "Verify");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), verify, 6);
	g_signal_connect(G_OBJECT(verify), "clicked", G_CALLBACK(check),NULL);
	
	separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 7);
	gtk_widget_show(GTK_WIDGET(separator));
	
	hint = gtk_tool_button_new_from_stock(GTK_STOCK_ABOUT);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(hint), "Hint");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), hint, 8);
	g_signal_connect(G_OBJECT(hint), "clicked", G_CALLBACK(helpMe),NULL);
	
	gtk_widget_show(toolbar);
}
/*Tool for user enterd sudoku option*/

void make_toolbar(GtkBox *box){
	GtkWidget *toolbar;
	GtkToolItem *bNew, *separator, *solve_btn;

	toolbar = gtk_toolbar_new();
	gtk_widget_set_name(toolbar, "toolbar");
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	gtk_box_pack_start(box, toolbar, 0, 0, 0);
	/*Making new button*/
	bNew = gtk_menu_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(bNew), " Generate New Game");
	g_signal_connect(G_OBJECT(bNew), "clicked", G_CALLBACK(new), 0); 
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), bNew, 0);
	gtk_widget_show(GTK_WIDGET(bNew));
	separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 1);
	gtk_widget_show(GTK_WIDGET(separator));

	
	/*Making solve button */
	solve_btn = gtk_tool_button_new_from_stock(GTK_STOCK_APPLY);
	g_signal_connect(G_OBJECT(solve_btn), "clicked",G_CALLBACK(get_element), NULL);
	gtk_tool_button_set_label(GTK_TOOL_BUTTON(solve_btn), "Solve!");
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(solve_btn), "Solution");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), solve_btn, 2);
	gtk_widget_show(GTK_WIDGET(solve_btn));
        
        separator = gtk_separator_tool_item_new();
	gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(separator), TRUE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), separator, 3);
	gtk_widget_show(GTK_WIDGET(separator)); 
	   
	gtk_widget_show(toolbar);
}

/*It will build window for PLAY mode sudoku*/
void build() {
	int i, j;
	GtkWidget *window, *vbox, *separator, *hbox;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "Enter sudoku");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	vbox = gtk_vbox_new(0,0);
	make_toolbar(GTK_BOX(vbox));
	for(i = 0; i < sudoku.type; i++) {
		hbox = gtk_hbox_new(0,0);
		for(j = 0; j < sudoku.type; j++) {
			wid[i][j] = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]), 2);
			gtk_widget_set_size_request(wid[i][j], 25, 25);
			gtk_box_pack_start(GTK_BOX(hbox), wid[i][j], 0, 0, 0);
			if((j+1) % (int)(sqrt(sudoku.type)) == 0) {
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox), separator, 0, 0, 0);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);
		if((i+1) % (int)(sqrt(sudoku.type)) == 0) {
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox), separator, 0, 0, 0);
		}
	}
	gtk_container_add(GTK_CONTAINER(window), vbox);	
	gtk_widget_show_all(window);
}

void build2() {
	int i, j;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	GtkWidget *window, *vbox, *separator, *hbox;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	vbox = gtk_vbox_new(0,0);
	make_toolbar_play(GTK_BOX(vbox));
	for(i = 0; i < sudoku.type; i++) {
		hbox = gtk_hbox_new(0,0);
		for(j = 0; j < sudoku.type; j++) {
			wid[i][j] = gtk_entry_new();
			if(sudoku.type < 16)
				gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]), 1);
			else
				gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]), 2);
			gtk_widget_set_size_request(wid[i][j], 25, 25);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j], 0, 0, 0);
			if((j+1) % (int)(sqrt(sudoku.type)) == 0) {
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox), separator, 0, 0, 0);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox), hbox, 0, 0, 0);
		if((i+1) % (int)(sqrt(sudoku.type)) == 0) {
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox), separator, 0, 0, 0);
		}
	}
	gtk_container_add(GTK_CONTAINER(window),vbox);
	if(sudoku.type != 25) {
		for (int i = 0; i < sudoku.type; i = i + sqrt(sudoku.type))
			Diagonal_box(i, i);
		fillit(0, 0);
	}
	show();
	for(i = 0;i < sudoku.type; i++) {
		for(j = 0;j < sudoku.type; j++) {
			char c[3];
			if(sudoku.puzzle[i][j]) {
				sprintf(c,"%d",sudoku.puzzle[i][j]);
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), c);
				gtk_widget_modify_bg(GTK_WIDGET(wid[i][j]), GTK_STATE_NORMAL,&color);
				gtk_entry_set_editable(GTK_ENTRY(wid[i][j]), FALSE);
			}
		}
	}
	
	gtk_widget_show_all(window);
}
void build25() {
	FILE* fp;
	int i = 0,j =0;
	fp = fopen("sudo25.txt", "r");
	if (fp == NULL ){
		return;
	}
	char line[100];
	while(j < 25) {
		fgets(line, 100, fp);
		help[i][j] = sudoku.puzzle[j][i] = atoi((strtok(line," ")));
		i++;
		while(i < 25) {
			help[i][j] = sudoku.puzzle[j][i]=atoi((strtok(NULL," ")));
			i++;
		}
		i = 0;
		j++;
	}
	build2();
	fclose(fp);
}
/*For 4*4 sudoku*/
void b4() {
	if(sudoku.type) 
		init();
	sudoku.type = 4;
	build2();
}
/*For 9*9 sudoku*/
void b9() {
	if(sudoku.type) 
		init();
	sudoku.type = 9;
	build2();
}
/*For 16*16 sudoku*/
void b16() {
	if(sudoku.type) 
		init();
	sudoku.type = 16;
	build2();
}
/*For 25*25 sudoku*/
void b25() {
	if(sudoku.type) 
		init();
	sudoku.type = 25;
	build25();
}

void Easy(){
	switch(sudoku.type) {
		case 4 :
			diffi = 4;
			b4();
			break;
		case 9 :
			diffi = 30;
			b9();
			break;
		case 16 :
			diffi = 60;
			b16(); 
			break;
		case 25 :
			diffi = 90;
			b25(); 
			break;
	}
	
}
void Medium(){
	switch(sudoku.type) {
		case 4 :
			diffi = 7;
			b4();
			break;
		case 9 :
			diffi = 40;
			b9();
			break;
		case 16 :
			diffi = 90;
			b16(); 
			break;
		case 25 :
			diffi = 105;
			b25(); 
			break;
	}
}
void Hard(){
	switch(sudoku.type) {
		case 4 :
			diffi = 10;
			b4();
			break;
		case 9 :
			diffi = 50;
			b9();
			break;
		case 16 :
			diffi = 150;
			b16(); 
			break;
		case 25 :
			diffi = 120;
			b25(); 
			break;
	}
}
void diff() {
	GtkWidget *window, *easy, *medium, *hard, *vbox,*load;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	vbox = gtk_vbox_new(0,0);
	easy = gtk_button_new_with_label("Easy");
	gtk_widget_set_size_request( easy, TOOL_X, TOOL_Y );
	g_signal_connect (easy, "clicked", G_CALLBACK(Easy), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), easy, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(easy), GTK_STATE_NORMAL, &color);
	
	medium = gtk_button_new_with_label("Medium");
	gtk_widget_set_size_request(medium, TOOL_X, TOOL_Y );
	g_signal_connect (medium, "clicked", G_CALLBACK(Medium), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), medium, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(medium), GTK_STATE_NORMAL, &color);
	
	hard = gtk_button_new_with_label("Hard");
	gtk_widget_set_size_request( hard, TOOL_X, TOOL_Y );
	g_signal_connect (hard, "clicked", G_CALLBACK(Hard), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), hard, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(hard), GTK_STATE_NORMAL, &color);
	
	load = gtk_button_new_with_label("Resume");
	gtk_widget_set_size_request(load, TOOL_X, TOOL_Y);
	g_signal_connect(load, "clicked", G_CALLBACK(loadFile), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), load, 0, 5, 5);	
	gtk_widget_modify_bg(GTK_WIDGET(load), GTK_STATE_NORMAL,&color);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

void Enter4(){
	sudoku.type = 4;
	build();
	new();
}
void Enter9(){
	sudoku.type = 9;
	build();
	new();
}
void Enter16(){
	sudoku.type = 16;
	build();
	new();	
}
void Enter25(){
	sudoku.type = 25;
	build();
	new();
}
/*Building menu window for ENTER SUDOKU MODE*/
void enter() {
	GtkWidget *window,*btn4, *btn9, *btn16, *btn25,*vbox;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "Enter sudoku");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	vbox = gtk_vbox_new(0,0);
	
	btn4 = gtk_button_new_with_label("4 x 4");
	gtk_widget_set_size_request( btn4, TOOL_X, TOOL_Y );
	g_signal_connect (btn4, "clicked", G_CALLBACK(Enter4), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn4, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn4), GTK_STATE_NORMAL, &color);
	
	btn9 = gtk_button_new_with_label("9 x 9");
	gtk_widget_set_size_request(btn9, TOOL_X, TOOL_Y );
	g_signal_connect (btn9, "clicked", G_CALLBACK(Enter9), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn9, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn9), GTK_STATE_NORMAL, &color);
	
	btn16 = gtk_button_new_with_label("16 x 16");
	gtk_widget_set_size_request( btn16, TOOL_X, TOOL_Y );
	g_signal_connect (btn16, "clicked", G_CALLBACK(Enter16), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn16, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn16), GTK_STATE_NORMAL, &color);
	
	btn25 = gtk_button_new_with_label("25 x 25");
	gtk_widget_set_size_request( btn25, TOOL_X, TOOL_Y );
	g_signal_connect (btn25, "clicked", G_CALLBACK(Enter25), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn25, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn25), GTK_STATE_NORMAL, &color);
	
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
}
/*Call to diff window*/
void e4() {
	sudoku.type = 4;
	diff();
}
void e9() {
	sudoku.type = 9;
	diff();
}
void e16() {
	sudoku.type = 16;
	diff();
}
void e25() {
	sudoku.type = 25;
	diff();
}
/*Menu window for play sudoku option*/
void play(GtkWidget* widget, gpointer data) {
	GtkWidget *window,*btn4, *btn9, *btn16, *btn25,*vbox;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	vbox = gtk_vbox_new(0,0);
	
	btn4 = gtk_button_new_with_label("4 x 4");
	gtk_widget_set_size_request( btn4, TOOL_X, TOOL_Y );
	g_signal_connect (btn4, "clicked", G_CALLBACK(e4), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn4, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn4), GTK_STATE_NORMAL, &color);
	
	btn9 = gtk_button_new_with_label("9 x 9");
	gtk_widget_set_size_request(btn9, TOOL_X, TOOL_Y );
	g_signal_connect (btn9, "clicked", G_CALLBACK(e9), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn9, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn9), GTK_STATE_NORMAL, &color);
	
	btn16 = gtk_button_new_with_label("16 x 16");
	gtk_widget_set_size_request( btn16, TOOL_X, TOOL_Y );
	g_signal_connect (btn16, "clicked", G_CALLBACK(e16), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn16, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn16), GTK_STATE_NORMAL, &color);
	
	btn25 = gtk_button_new_with_label("25 x 25");
	gtk_widget_set_size_request( btn25, TOOL_X, TOOL_Y );
	g_signal_connect (btn25, "clicked", G_CALLBACK(e25), NULL);
	gtk_box_pack_start(GTK_BOX(vbox), btn25, 0, 5, 5);
	gtk_widget_modify_bg(GTK_WIDGET(btn25), GTK_STATE_NORMAL, &color);
	
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}
void aboutMe() {
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	GtkWidget *window, *label, *vbox;
	vbox = gtk_vbox_new(0,0);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
	gtk_window_set_title(GTK_WINDOW(window), "About");
	gtk_widget_modify_bg(GTK_WIDGET(window), GTK_STATE_NORMAL,&color);
	label = gtk_label_new("Welcome to SUDOKU\n-----------------------------------\nCreated By :PRASAD RATHOD\nMIS :111603052\nCollege Of Engineering,\nPune!");
	gtk_box_pack_start(GTK_BOX(vbox), label, 0, 5, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
}

int main (int argc, char* argv[]) {
	
	gtk_init(&argc, &argv);
	init();
	GtkWidget *bQuit, *UserInp, *bPlay, *window, *vbox, *lable, *about;
	GdkColor color;
	gdk_color_parse("skyblue", &color);
	vbox = gtk_vbox_new(0,0);
	window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 200);
	gtk_window_set_title(GTK_WINDOW(window), "Menu");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect( window, "delete-event", G_CALLBACK(gtk_main_quit), NULL );
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);
	
	lable = gtk_label_new("Welcome...!");
	gtk_box_pack_start(GTK_BOX(vbox), lable, 0, 5, 5);
	
	UserInp = gtk_button_new_with_label( "Enter your sudoku" );
	gtk_widget_set_size_request( UserInp, TOOL_X, TOOL_Y );
	g_signal_connect (UserInp, "clicked", G_CALLBACK(enter), NULL);
	gtk_widget_modify_bg(GTK_WIDGET(UserInp), GTK_STATE_NORMAL,&color);
	
	bPlay = gtk_button_new_with_label( "Play" );
	gtk_widget_set_size_request( bPlay, TOOL_X, TOOL_Y );
	g_signal_connect (bPlay, "clicked", G_CALLBACK(play), NULL);
	gtk_widget_modify_bg(GTK_WIDGET(bPlay), GTK_STATE_NORMAL,&color);
	
	about = gtk_button_new_with_label( "About" );
	gtk_widget_set_size_request( about, TOOL_X, TOOL_Y );
	g_signal_connect (about, "clicked", G_CALLBACK(aboutMe), NULL);
	gtk_widget_modify_bg(GTK_WIDGET(about), GTK_STATE_NORMAL,&color);
	
	bQuit = gtk_button_new_with_label( "Quit" );
	gtk_widget_set_size_request( bQuit, TOOL_X, TOOL_Y );
	g_signal_connect (bQuit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
	gtk_widget_modify_bg(GTK_WIDGET(bQuit), GTK_STATE_NORMAL,&color);
			
	gtk_box_pack_start(GTK_BOX(vbox), UserInp, 0, 5, 5);
	gtk_box_pack_start(GTK_BOX(vbox), bPlay, 0, 5, 5);
	gtk_box_pack_start(GTK_BOX(vbox), about, 0, 5, 5);
	gtk_box_pack_start(GTK_BOX(vbox), bQuit, 0, 5, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

