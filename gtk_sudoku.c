#include <gtk/gtk.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
int n;
static GtkWidget *wid[25][25], *window;
void menu_event(GtkWidget *widget, gpointer data) {
	if(strcmp(gtk_menu_item_get_label(GTK_MENU_ITEM(widget)),"New")) {
		for(int i = 0; i < 9; i++)
			for(int j = 0; j < 9; j++) {
				a[i][j] = 0;	
				gtk_entry_set_text(GTK_ENTRY(wid[i][j]), "");
			}
	}
	
}

void get_element(GtkWidget *window, gpointer data) {
	for(int i=0; i<n; i++) 
		for(int j=0; j<n; j++)
			a[i][j]=atoi(gtk_entry_get_text(GTK_ENTRY(wid[i][j])));
		sudokusolver();


}

int main (int argc, char* argv[]) {
	
	puts("Enter the value");
	scanf("%d", &n);
	const char* file[] = {"New","Open","Exit"};
	gtk_init(&argc, &argv);
	GtkWidget *vbox,*hbox,*separator,*button,*file_menu,*menu_bar,*menu_item;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "sudoku");
	vbox = gtk_vbox_new(0,0);
	file_menu = gtk_menu_new();
	menu_bar = gtk_menu_bar_new();
	menu_item = gtk_menu_item_new_with_label("File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item),file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar),menu_item);
	for(int i = 0; i < 3; i++) {
		menu_item = gtk_menu_item_new_with_label(file[i]);
		gtk_menu_shell_append(GTK_MENU_SHELL(file_menu),menu_item);
		g_signal_connect(menu_item,"activate",G_CALLBACK(menu_event),NULL);
	}
	gtk_box_pack_start(GTK_BOX(vbox),menu_bar,0,0,0);
	
	for(int i = 0; i < n; i++) {
		hbox = gtk_hbox_new(0,0);
		for(int j = 0; j < n; j++) {
			wid[i][j] = gtk_entry_new();
			gtk_entry_set_max_length(GTK_ENTRY(wid[i][j]),1);
			gtk_widget_set_size_request(wid[i][j],20,20);
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
	button = gtk_button_new_with_label("Solve Sudoku");
	g_signal_connect(button, "clicked", G_CALLBACK(get_element),NULL);
	gtk_box_pack_start(GTK_BOX(vbox), button, 0,0,0);
	
	g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}