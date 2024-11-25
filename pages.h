
#include <gtk/gtk.h>
#ifndef PAGE_H   // Check if PAGE_H is not defined
#define PAGE_H   // Define PAGE_H

void show_main_menu_page(GtkWidget *starting_window);
void show_difficulty_page(GtkWidget *main_menu_window);
void show_single_player_page(GtkWidget *difficulty_window, char *difficulty);
void show_double_player_page(GtkWidget *main_menu_window);

void load_css(void);
void on_start_clicked(GtkWidget *widget, gpointer data);
void initialize_gui(void);
// void play_song(const char *filename);
// void play_sound(const char *filename);

#endif // PAGE_H  // End of header guard
