#include <stdio.h> // Include the standard I/O library for file operations
#include <stdlib.h> 
#include "constants.h"
#include "pages.h"

extern GtkWidget *fixed, *background, *title;
GtkWidget *double_player_window; // Declare the double player window

// Function to display the double player page
void show_double_player_page(GtkWidget *main_menu_window) {
    static char player_x[10] = "PLAYER X"; // Default name for Player X
    static char player_o[10] = "PLAYER O"; // Default name for Player O
    
    // Load custom player names for score box and status label display
    load_players_name(player_x, player_o);

    // Allocate memory for game data
    game_data = (GameData *)malloc(sizeof(GameData));
    if (game_data == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Failed to allocate memory for game data\n");
        return;
    }

    // Hide the main menu window
    gtk_widget_hide(main_menu_window);

    // Create the double player window
    double_player_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(double_player_window), "TIC - TAC - TOE"); // Set the window title
    gtk_window_set_default_size(GTK_WINDOW(double_player_window), 480, 800); // Set the default window size
    gtk_container_set_border_width(GTK_CONTAINER(double_player_window), 40); // Add padding around the window content
    gtk_window_set_resizable(GTK_WINDOW(double_player_window), FALSE); // Disable window resizing

    // Create a fixed container for widget positioning
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(double_player_window), fixed);

    // Create the title label for the game
    title = gtk_label_new("TIC - TAC - TOE");
    gtk_widget_set_name(title, "title_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), title, 0, 0);

    // Create the status label to display the current player's turn
    status_label = gtk_label_new("Player X's turn");
    gtk_widget_set_name(status_label, "status_label");  // Set a name for CSS targeting
    gtk_fixed_put(GTK_FIXED(fixed), status_label, 0, 40);

    // Create a grid layout for the Tic-Tac-Toe board
    GtkWidget *grid = gtk_grid_new(); 
    gtk_widget_set_name(grid, "tic-tac-toe-grid"); // Set a name for CSS targeting
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); // Center the grid horizontally
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); // Center the grid vertically
    gtk_grid_set_row_spacing(GTK_GRID(grid), 20); // Add spacing between rows
    gtk_grid_set_column_spacing(GTK_GRID(grid), 20); // Add spacing between columns
    gtk_fixed_put(GTK_FIXED(fixed), grid, 40, 144);

    // Make rows and columns expand evenly within the grid
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

    // Create buttons for the Tic-Tac-Toe grid
    for (int i = 0; i < 9; i++) {
        buttons[i] = gtk_button_new_with_label(" "); // Initialize button with an empty label
        gtk_widget_set_name(buttons[i], "grid-button"); // Set a name for CSS targeting
        gtk_widget_set_size_request(buttons[i], 96, 96); // Set button size
        gtk_widget_set_hexpand(buttons[i], TRUE); // Allow horizontal expansion
        gtk_widget_set_vexpand(buttons[i], TRUE); // Allow vertical expansion

        // Attach the button to the grid at the appropriate row and column
        gtk_grid_attach(GTK_GRID(grid), buttons[i], i % 3, i / 3, 1, 1);

        // Connect the button's "clicked" signal to the callback function
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked2), GINT_TO_POINTER(i));
        gtk_widget_set_sensitive(buttons[i], TRUE); // Ensure the button is initially clickable
    } 

    // Create and configure the X scoreboard
    GtkWidget *score_x_label = gtk_label_new("PLAYER X\n        0"); // Display player name and score
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-box"); // Add CSS class for styling
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-x"); // Add specific class for Player X
    gtk_widget_set_size_request(score_x_label, 86, 70); // Set scoreboard size
    game_data->score_x_label = score_x_label; // Store the label in game data
    gtk_fixed_put(GTK_FIXED(fixed), score_x_label, 40, 520); // Position the scoreboard

    // Create and configure the O scoreboard
    GtkWidget *score_o_label = gtk_label_new("PLAYER O\n        0"); // Display player name and score
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-box"); // Add CSS class for styling
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-o"); // Add specific class for Player O
    gtk_widget_set_size_request(score_o_label, 86, 70); // Set scoreboard size
    game_data->score_o_label = score_o_label; // Store the label in game data
    gtk_fixed_put(GTK_FIXED(fixed), score_o_label, 280, 520); // Position the scoreboard

    // Create and configure the Draw scoreboard
    GtkWidget *score_draw_label = gtk_label_new("DRAW\n    0"); // Display draw score
    gtk_style_context_add_class(gtk_widget_get_style_context(score_draw_label), "score-box"); // Add CSS class for styling
    gtk_style_context_add_class(gtk_widget_get_style_context(score_draw_label), "score-draw"); // Add specific class for draws
    gtk_widget_set_size_request(score_draw_label, 86, 70); // Set scoreboard size
    game_data->score_draw_label = score_draw_label; // Store the label in game data
    gtk_fixed_put(GTK_FIXED(fixed), score_draw_label, 160, 520); // Position the scoreboard

    // Create the "Back" button with an arrow image
    GtkWidget *back_button = gtk_button_new_with_label("Back");
    GtkWidget *arrow = gtk_image_new_from_file("images/backArrow.png"); // Load the back arrow image
    gtk_widget_set_name(back_button, "back_button");  // Set a name for CSS targeting
    gtk_widget_set_name(arrow, "arrow");  // Set a name for CSS targeting
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), double_player_window); // Connect the back button to its callback
    gtk_button_set_image(GTK_BUTTON(back_button), arrow); // Add the arrow image to the back button
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 8, 640); // Position the back button

    // Show all widgets in the double player window
    gtk_widget_show_all(double_player_window);
}
