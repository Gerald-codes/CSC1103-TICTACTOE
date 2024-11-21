#include <gtk/gtk.h> 
#include <stdio.h> // Include the standard I/O library for file operations
#include <stdlib.h> 
#include "constants.h"
#include "pages.h"

GtkWidget *double_player_window;

void show_double_player_page(GtkWidget *main_menu_window) {
    static char player_x[10] = "Player X";
    static char player_o[10] = "Player O";
    
    load_players_name(player_x, player_o);

    game_data = (GameData *)malloc(sizeof(GameData));
    if (game_data == NULL) {
        // Handle allocation failure
        fprintf(stderr, "Failed to allocate memory for game data\n");
        return;
    }

    // Hide the main menu window
    gtk_widget_hide(main_menu_window);

    // Create the double player window
    double_player_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(double_player_window), "TIC - TAC - TOE");
    gtk_window_set_default_size(GTK_WINDOW(double_player_window), 600, 1000);
    gtk_container_set_border_width(GTK_CONTAINER(double_player_window), 40);

    GtkWidget *fixed, *background, *title, *mode;

    // Fixed container to allow positioning of widgets
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(double_player_window), fixed);

    // Create the Title label
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    status_label = gtk_label_new("Player X's turn");
    gtk_widget_set_name(status_label, "status_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), status_label, 0, 50);


    // Create the grid
    GtkWidget *grid = gtk_grid_new(); 
    gtk_widget_set_name(grid, "tic-tac-toe-grid"); // Set the name of the grid widget
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); // Center the grid horizontally
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); // Center the grid vertically
    gtk_grid_set_row_spacing(GTK_GRID(grid), 25); // Set the row spacing to 25 pixels
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25); // Set the column spacing to 25 pixels
    gtk_fixed_put(GTK_FIXED(fixed), grid, 50, 180);

    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    for (int i = 0; i < 9; i++) {
        buttons[i] = gtk_button_new_with_label(" "); 
        gtk_widget_set_name(buttons[i], "grid-button"); // Add the grid-button class
        gtk_widget_set_size_request(buttons[i], 120, 120); // Set the size of the button
        gtk_widget_set_hexpand(buttons[i], TRUE); // Set the horizontal expand property of the button
        gtk_widget_set_vexpand(buttons[i], TRUE); // Set the vertical expand property of the button

        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 3, i / 3, 1, 1); // Attach the button to the grid
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i)); // Connect the "clicked" signal of the button to the button_clicked function
        gtk_widget_set_sensitive(buttons[i], TRUE);
    } 

    // X Scoreboard
    GtkWidget *score_x_label = gtk_label_new("PLAYER X\n       0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-x");
    gtk_widget_set_size_request(score_x_label, 107, 87); // Set the size of the label
    game_data->score_x_label = score_x_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_x_label, 50, 650);


    // O Scoreboard
    GtkWidget *score_o_label = gtk_label_new("PLAYER O\n        0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-o");
    gtk_widget_set_size_request(score_o_label, 107, 87); // Set the size of the label
    game_data->score_o_label = score_o_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_o_label, 350, 650);

    // Ties Scoreboard
    GtkWidget *score_ties_label = gtk_label_new("TIES\n    0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_ties_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_ties_label), "score-ties");
    gtk_widget_set_size_request(score_ties_label, 107, 87); // Set the size of the label
    game_data->score_ties_label = score_ties_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_ties_label, 200, 650);

    // Create the "Back" button 
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *arrow = gtk_image_new_from_file("images/backArrow.png");
    gtk_widget_set_name(back_button, "back_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), double_player_window);
    gtk_button_set_image(GTK_BUTTON(back_button),arrow);
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 10, 800);

    // Show all widgets in the double player window
    gtk_widget_show_all(double_player_window);

    // Connect the delete-event signal to quit the application when the double player window is closed
    g_signal_connect(double_player_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    reset_board();
}