
#include <gtk/gtk.h>
#ifndef PAGE_H   // Check if HEADER_NAME_H is not defined
#define PAGE_H   // Define HEADER_NAME_H




void show_main_menu_page(GtkWidget *starting_window);
void show_difficulty_page(GtkWidget *main_menu_window);
void show_single_player_page(GtkWidget *difficulty_window, char *difficulty);
void show_double_player_page(GtkWidget *main_menu_window);


#endif // HEADER_NAME_H  // End of header guard