#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include<glib.h>
#include<glib/gprintf.h>
#define TOOL_X 94
#define TOOL_Y 50
#define BLUE "\x1B[34m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"
static int a[25][25] = {0};
int n;
static GtkWidget *wid[25][25], *window;
void save(GtkWidget *widget, gpointer data) {
	FILE* savefile = NULL;
	savefile = fopen("save.txt", "w");
	if(!savefile) {
		puts("Unable to save file...!");
		return;
	}
	

}
void close_window(GtkWidget *widget, gpointer window)
{
    gtk_widget_destroy(GTK_WIDGET(window));
}
void new(GtkWidget *widget, gpointer data) {
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++) {
				a[i][j] = 0;	
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), "");
			}
	
}
int square(int i, int j, int k) {
	int r = ceil((i+1)/sqrt(n));
	int c = ceil((j+1)/sqrt(n));
	for(int g = (r-1)*sqrt(n); g < ((r-1)*sqrt(n)+sqrt(n)); g++)
		for(int h = (c-1)*sqrt(n); h < ((c-1)*sqrt(n)+sqrt(n)); h++)
			if(a[g][h] == k)
				return 0;
	return 1;
}
int inrow(int row, int num) {
	for (int col = 0; col < n; col++) {
		if (a[row][col] == num)
			return 1;
	}
	return 0;
}

int incol(int col, int num) {
	for (int row = 0; row < n; row++) {
		if (a[row][col] == num)
			return 1;
	}
	return 0;
}

int isvalid(int row,int col, int num) {
	return ((!inrow(row,num) && !incol(col,num)) && square(row,col,num));
}
int sudokuSolver(int row, int col) {
	if (row == n)
		return 1;
	if (a[row][col]) {
		if (col == n- 1) {
			if(sudokuSolver(row + 1, 0))
				return 1;
		}
		else {
			if(sudokuSolver(row, col + 1))
				return 1;
		}
	}
	for (int num = 1; num <= n ; num++) {
	
		if(isvalid(row, col,num)) {
			a[row][col] = num;
			if (col == n - 1) {
				if(sudokuSolver(row + 1, 0))
					return 1;
			}
			else {
				if(sudokuSolver(row, col + 1))
					return 1;
			}
			//if failed to find num
			a[row][col] = 0;
		}
	}
	return 0;
}


void get_element(GtkWidget *window, gpointer data) {
for(int i=0; i<n; i++) 
		for(int j=0; j<n; j++)
			a[i][j]=atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
		if(sudokuSolver(0,0)) {
			for(int i=0;i<n;i++){
				for(int j=0;j<n;j++){
					char c[2];
					sprintf(c,"%d",a[i][j]);
					gtk_entry_set_text(GTK_ENTRY(wid[i][j]),c);}
		}}
		else
			puts("It is not solvable..!");


}
void on_close(GtkWidget *widget, gpointer data){};
void make_sudoku(GtkBox *box) {
	GtkWidget *vbox,*hbox,*separator;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	vbox = gtk_vbox_new(0,0);
	for(int i = 0; i < n; i++) {
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < n; j++) {
			wid[i][j] = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]),1);
			gtk_entry_set_editable(GTK_ENTRY(wid[i][j]),FALSE);
			gtk_widget_set_size_request(wid[i][j],25,25);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,0,0);
			if((j+1)%(int)(sqrt(n)) == 0) {
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,0);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%(int)(sqrt(n)) == 0) {
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,0);
		}
	}

	g_signal_connect(window, "delete-event", G_CALLBACK(close_window), NULL);
	
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
	/*GtkWidget *table;
	table = gtk_table_new(n,n,1);
	for(int row = sqrt(n)-1; row < n; row += sqrt(n)) {
		gtk_table_set_row_spacing(GTK_TABLE(table), row, 2);
		gtk_table_set_col_spacing(GTK_TABLE(table), row, 2);
	}
	gtk_container_add(GTK_CONTAINER(window),table);
	gtk_widget_show(table);*/
	
}


void make_toolbar(GtkBox *box){
	GtkWidget   *toolbar;
	GtkToolItem *new_btn;
	GtkToolItem *separator;
	GtkToolItem *verify_btn;
	GtkToolItem *clear_btn;
	GtkToolItem *solve_btn;

	toolbar = gtk_toolbar_new();
	gtk_widget_set_name(toolbar, "toolbar");
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_BOTH);
	gtk_box_pack_start(box, toolbar, TRUE, TRUE, 0);
	/*Making new button*/
	new_btn = gtk_menu_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_tool_item_set_tooltip_text(GTK_TOOL_ITEM(new_btn), " Generate New Game");
	g_signal_connect(G_OBJECT(new_btn), "clicked",                            
			G_CALLBACK(new), 0); 
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new_btn, 0);
	gtk_widget_show(GTK_WIDGET(new_btn));
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

void userSol() {
	
	GtkWidget *vbox,*hbox,*separator;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	vbox = gtk_vbox_new(0,0);
	make_toolbar(GTK_BOX(vbox));
	for(int i = 0; i < n; i++) {
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < n; j++) {
			wid[i][j] = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]),1);
			gtk_widget_set_size_request(wid[i][j],25,25);
			gtk_box_pack_start(GTK_BOX(hbox),wid[i][j],0,0,0);
			if((j+1)%(int)(sqrt(n)) == 0) {
				separator = gtk_vseparator_new();
				gtk_box_pack_start(GTK_BOX(hbox),separator,0,0,0);
			}
		}
		gtk_box_pack_start(GTK_BOX(vbox),hbox,0,0,0);
		if((i+1)%(int)(sqrt(n)) == 0) {
			separator = gtk_hseparator_new();
			gtk_box_pack_start(GTK_BOX(vbox),separator,0,0,0);
		}
	}

	g_signal_connect(window, "delete-event", G_CALLBACK(close_window), NULL);
	
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
}
void play(GtkWidget* widget, gpointer data) {
	GtkWidget *vbox,*hbox,*separator;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window),250,100);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	vbox = gtk_vbox_new(0,0);
	make_sudoku(GTK_BOX(vbox));
	
	g_signal_connect(window, "delete-event", G_CALLBACK(close_window), NULL);
	
	//gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
}
int main (int argc, char* argv[]) {
	
	puts(BLUE "Enter the value" RESET);
	while(scanf("%d", &n) != -1 ) {
		if(!(n == 4 || n == 9 || n==16 || n==2)) {
			puts(RED "Invalid Input" RESET);
		
		}
		else {
			gtk_init(&argc, &argv);
			GtkWidget *bQuit, *UserInp, *bPlay,*window,*vbox;
			vbox = gtk_vbox_new(0,0);
			window = gtk_window_new( GTK_WINDOW_TOPLEVEL );
			gtk_window_set_default_size(GTK_WINDOW(window),200,200);
			gtk_window_set_title(GTK_WINDOW(window), "Menu");
			g_signal_connect( window, "delete-event", G_CALLBACK(gtk_main_quit), NULL );
			gtk_container_set_border_width(GTK_CONTAINER(window), 10);

			UserInp = gtk_button_new_with_label( "Get Answer" );
			gtk_widget_set_size_request( UserInp, TOOL_X, TOOL_Y );
			g_signal_connect (UserInp, "clicked", G_CALLBACK(userSol), NULL);
	
			bPlay = gtk_button_new_with_label( "Play" );
			gtk_widget_set_size_request( bPlay, TOOL_X, TOOL_Y );
			g_signal_connect (bPlay, "clicked", G_CALLBACK(play), NULL);
	
			bQuit = gtk_button_new_with_label( "Quit" );
			gtk_widget_set_size_request( bQuit, TOOL_X, TOOL_Y );
			g_signal_connect (bQuit, "clicked", G_CALLBACK(gtk_main_quit), NULL);
			gtk_box_pack_start(GTK_BOX(vbox),UserInp,0,0,0);
			gtk_box_pack_start(GTK_BOX(vbox),bPlay,0,0,0);
			gtk_box_pack_start(GTK_BOX(vbox),bQuit,0,0,0);
			gtk_container_add(GTK_CONTAINER(window),vbox);
			gtk_widget_show_all(window);
			gtk_main();
			return 0;
		}
	}
	
	

}