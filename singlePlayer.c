#include <gtk/gtk.h> 
#include <stdio.h> // Include the standard I/O library for file operations
#include <stdlib.h> 
#include "constants.h"
#include "pages.h"
#include "backend.h"
#include <time.h>

extern GtkWidget *difficulty_window;
GtkWidget *single_player_window;
char difficulty_mode[10]; // Declare a string called difficulty_mode
LinearRegressionModel model;

void show_single_player_page(GtkWidget *difficulty_window, char *difficulty) {
    if (strcmp(difficulty,"EASY") == 0){
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
        parse_dataset(&model);  // Parse the dataset
    }else if(strcmp(difficulty, "MEDIUM") == 0){
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
    }else{
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
    }

    static char player_x[10] = "X (YOU)";
    static char player_o[10] = "O (CPU)";

    load_players_name(player_x, player_o);

    game_data = (GameData *)malloc(sizeof(GameData));
    if (game_data == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for game data\n");
        return;
    }

    // Hide the main menu window
    gtk_widget_hide(difficulty_window);

    // Create the single player window
    single_player_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(single_player_window), "TIC - TAC - TOE");
    gtk_window_set_default_size(GTK_WINDOW(single_player_window), 480, 800);
    gtk_container_set_border_width(GTK_CONTAINER(single_player_window), 40);

    GtkWidget *fixed, *background, *title, *mode;

    // Fixed container to allow positioning of widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(single_player_window), fixed);

    // Create the Title label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Create the mode label
    char status_label[50];
    snprintf(status_label, sizeof(status_label), "SINGLE PLAYER - %s MODE", difficulty_mode);
    mode = gtk_label_new(status_label);
    gtk_widget_set_name(mode, "mode_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), mode, 0, 40);

    // Create the grid
    GtkWidget *grid = gtk_grid_new(); 
    gtk_widget_set_name(grid, "tic-tac-toe-grid"); // Set the name of the grid widget
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); // Center the grid horizontally
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); // Center the grid vertically
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20); // Set the row spacing to 25 pixels
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20); // Set the column spacing to 25 pixels
    gtk_fixed_put(GTK_FIXED(fixed), grid, 40, 144);


    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    for (int i = 0; i < 9; i++) {
        buttons[i] = gtk_button_new_with_label(" "); 
        gtk_widget_set_name(buttons[i], "grid-button"); // Add the grid-button class
        gtk_widget_set_size_request(buttons[i], 96, 96); // Set the size of the button
        gtk_widget_set_hexpand(buttons[i], TRUE); // Set the horizontal expand property of the button
        gtk_widget_set_vexpand(buttons[i], TRUE); // Set the vertical expand property of the button

        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 3, i / 3, 1, 1); // Attach the button to the grid
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i)); // Connect the "clicked" signal of the button to the button_clicked function
        gtk_widget_set_sensitive(buttons[i], TRUE);
    } 

    // X Scoreboard
    GtkWidget *score_x_label = gtk_label_new("X (YOU)\n       0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-x");
    gtk_widget_set_size_request(score_x_label, 86, 70); // Set scoreboard size
    game_data->score_x_label = score_x_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_x_label, 40, 520); // Position the scoreboard


    // O Scoreboard
    GtkWidget *score_o_label = gtk_label_new("O (CPU)\n        0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-o");
    gtk_widget_set_size_request(score_o_label, 86, 70); // Set scoreboard size
    game_data->score_o_label = score_o_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_o_label, 280, 520); // Position the scoreboard

    // Draw Scoreboard
    GtkWidget *score_draw_label = gtk_label_new("DRAW\n    0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_draw_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_draw_label), "score-draw");
    gtk_widget_set_size_request(score_draw_label, 86, 70); // Set scoreboard size
    game_data->score_draw_label = score_draw_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_draw_label, 160, 520); // Position the scoreboard

    // Create the "Back" button 
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *arrow = gtk_image_new_from_file("images/backArrow.png");
    gtk_widget_set_name(back_button, "back_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), single_player_window);
    gtk_button_set_image(GTK_BUTTON(back_button),arrow);
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 8, 640);

    // Show all widgets in the single player window
    gtk_widget_show_all(single_player_window);

    // Connect the delete-event signal to quit the application when the single player window is closed
    g_signal_connect(single_player_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

}