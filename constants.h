#include <gtk/gtk.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include "pages.h"


#ifndef CONSTANTS_H
#define CONSTANTS_H

extern GtkWidget *main_menu_window;
extern GtkWidget *difficulty_window;


typedef struct { // Define a struct called GameData
    GtkWidget *score_x_label; // Declare a GtkWidget pointer called score_x_label
    GtkWidget *score_o_label; // Declare a GtkWidget pointer called score_o_label
    GtkWidget *score_ties_label; // Declare a GtkWidget pointer called score_ties_label
    GdkPixbuf *x_pixbuf; // Declare a GdkPixbuf pointer called x_pixbuf
    GdkPixbuf *o_pixbuf; // Declare a GdkPixbuf pointer called o_pixbuf
} GameData; // Name the struct GameData

extern GtkWidget *buttons[9]; // Shared button widgets
extern GtkWidget *back_button;
extern GtkWidget *end_button;
extern GtkWidget *status_label;

extern char player;             // Current player
extern char board[9];           // Board state
extern char player_x_label[10]; // Player X label
extern char player_o_label[10]; // Player O label

extern int score_x;             // Player X score
extern int score_o;             // Player O score
extern int score_ties;          // Ties score
extern int winning_indices[3];  // Winning indices

extern gboolean auto_reset_pending;
extern guint blink_timeout_id;
extern gboolean blink_state;
extern GameData *game_data;


extern gboolean blink_winner(gpointer data);
extern gboolean delayed_reset(gpointer data);

void load_players_name(char *name_x, char *name_o);
void update_scoreboard();

void reset_board(void);
void reset_board();

char check_winner();
gboolean auto_reset_board(gpointer data);

// Function to blink the winning indices
gboolean blink_winner(gpointer data);

void button_clicked(GtkWidget *widget, gpointer data);

void button_clicked2(GtkWidget *widget, gpointer data);

void cleanup_game_data();

// Callback function for the "Back" button
void on_back_button_clicked(GtkWidget *widget, gpointer data);

bool checkWin(char player);
bool checkTie();


#endif // HEADER_NAME_H  // End of header guard
