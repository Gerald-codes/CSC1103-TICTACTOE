//gcc -arch arm64 TTT_2player.c -o TTT_2player $(pkg-config --cflags --libs gtk+-3.0)
#include <gtk/gtk.h> 
#include <stdio.h> // Include the standard I/O library for file operations
#include <stdlib.h> 

typedef struct { // Define a struct called GameData
    GtkWidget *score_x_label; // Declare a GtkWidget pointer called score_x_label
    GtkWidget *score_o_label; // Declare a GtkWidget pointer called score_o_label
    GtkWidget *score_ties_label; // Declare a GtkWidget pointer called score_ties_label
    GdkPixbuf *x_pixbuf; // Declare a GdkPixbuf pointer called x_pixbuf
    GdkPixbuf *o_pixbuf; // Declare a GdkPixbuf pointer called o_pixbuf
} GameData; // Name the struct GameData

static char player = 'X';
static char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
static GtkWidget *buttons[9]; // Array to store the button widgets
static GtkWidget *back_button; // Declare a GtkWidget pointer called back_button
static GtkWidget *status_label; // Declare a GtkWidget pointer called status_label

/*
static GtkWidget *score_label_x; // Declare a GtkWidget pointer called score_label_x
static GtkWidget *score_label_o; // Declare a GtkWidget pointer called score_label_o
static GtkWidget *score_label_ties; // Declare a GtkWidget pointer called score_label_ties
*/
static int score_x = 0; // Declare an integer called score_x and set it to 0
static int score_o = 0; // Declare an integer called score_o and set it to 0
static int score_ties = 0; // Declare an integer called score_ties and set it to 0
static gboolean blink_state = FALSE; // Declare a gboolean called blink_state and set it to FALSE
static guint blink_timeout_id = 0; // Declare a guint called blink_timeout_id and set it to 0
static int winning_indices[3] = {-1, -1, -1}; // Declare an integer array called winning_indices and set it to {-1, -1, -1}
static GameData *game_data; // Declare a pointer to a GameData struct called game_data

void update_scoreboard() {
    char score_text[50];
    snprintf(score_text, sizeof(score_text), "PLAYER X\n         %d", score_x); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_x_label), score_text);
    snprintf(score_text, sizeof(score_text), "PLAYER O\n         %d", score_o); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_o_label), score_text);
    snprintf(score_text, sizeof(score_text), "TIES\n    %d", score_ties); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_ties_label), score_text);
}

void save_scores() {
    FILE *outfile_ptr;
    if ((outfile_ptr = fopen("scores.txt", "w")) == NULL) { // Open the file for writing
        printf("File could not be opened for writing\n"); // print an error message
        exit(2); // Exit the program
    }
    fprintf(outfile_ptr, "%d %d %d\n", score_x, score_o, score_ties); // Write the scores to the file
    fclose(outfile_ptr); // Close the file
}

void reset_board() {
    for (int i = 0; i < 9; i++) { // Loop through the board array
        board[i] = ' '; // Set each element to a space character
        gtk_button_set_label(GTK_BUTTON(buttons[i]), " "); // Set the label of the button to a space character
        GtkStyleContext *context = gtk_widget_get_style_context(buttons[i]); // Get the style context of the button
        gtk_style_context_remove_class(context, "blink"); // Remove the "blink" class from the button
    }
    player = 'X'; 
    gtk_label_set_text(GTK_LABEL(status_label), "Player X's turn"); // Set the text of the status_label widget
    if (blink_timeout_id != 0) { // Check if the blink_timeout_id is not 0
        g_source_remove(blink_timeout_id); // Remove the timeout
        blink_timeout_id = 0; // Set the blink_timeout_id to 0
    }
}

gboolean blink_winner(gpointer data) { 
    blink_state = !blink_state;
    for (int i = 0; i < 3; i++) { // Loop through the winning_indices array
        GtkStyleContext *context = gtk_widget_get_style_context(buttons[winning_indices[i]]); 
        if (blink_state) { // Check the blink_state
            gtk_style_context_add_class(context, "blink"); 
        } else { // If the blink_state is FALSE
            gtk_style_context_remove_class(context, "blink");
        }
    }
    return TRUE;
}

char check_winner() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2] && board[i * 3] != ' ') {
            winning_indices[0] = i * 3;
            winning_indices[1] = i * 3 + 1;
            winning_indices[2] = i * 3 + 2;
            return board[i * 3];
        }
    }
    // Check columns
    for (int i = 0; i < 3; i++) { // Loop through the columns
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6] && board[i] != ' ') { // Check if the columns are equal
            winning_indices[0] = i; // top cell of the column
            winning_indices[1] = i + 3; // middle cell of the column 
            winning_indices[2] = i + 6; // bottom cell of the column
            return board[i]; // Return the player
        }
    }
    // Check diagonals
    if (board[0] == board[4] && board[4] == board[8] && board[0] != ' ') { // Check the diagonal from top-left to bottom-right
        winning_indices[0] = 0; // top-left cell
        winning_indices[1] = 4; // middle cell
        winning_indices[2] = 8; // bottom-right cell
        return board[0]; // Return the player
    }
    if (board[2] == board[4] && board[4] == board[6] && board[2] != ' ') {
        winning_indices[0] = 2; // top-right cell
        winning_indices[1] = 4; // middle cell
        winning_indices[2] = 6; // bottom-left cell
        return board[2]; // Return the player
    }
    // Check for draw
    for (int i = 0; i < 9; i++) { 
        if (board[i] == ' ') { // Check if there are any empty cells
            return ' '; // Game is still ongoing
        }
    }
    return 'D'; // Draw
}

void button_clicked(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data); // Convert the data pointer to an integer
    if (board[index] == ' ') { // Check if the board at the index is empty
        board[index] = player;
        
        gtk_button_set_label(GTK_BUTTON(widget), &player); // Set the label of the button to the player

        // Set the name of the widget
        if (player == 'X') {
            gtk_widget_set_name(widget, "button-x"); //extract button-x style from css
        } else {
            gtk_widget_set_name(widget, "button-o"); //extract button-o style from css
        }

        char winner = check_winner();
        if (winner != ' ') { // Check if there is a winner
            char message[50];
            if (winner == 'D') { // Check if the game is a draw
                snprintf(message, sizeof(message), "It's a draw!"); 
                score_ties++;
            } else { // If there is a winner
                snprintf(message, sizeof(message), "Player %c wins!", winner);
                if (winner == 'X') { // Check if the winner is X
                    score_x++;
                } else { // If the winner is O
                    score_o++;
                }
                blink_timeout_id = g_timeout_add(500, blink_winner, NULL); // Add a timeout to blink the winning indices
            }
            gtk_label_set_text(GTK_LABEL(status_label), message); // Set the text of the status_label widget
            update_scoreboard(); // Update the scoreboard
            
            for (int i = 0; i < 9; i++) {
                gtk_widget_set_sensitive(buttons[i], FALSE); // Disable the button
            }
            
        } else {
            player = (player == 'X') ? 'O' : 'X'; // Switch players
            char message[50]; 
            snprintf(message, sizeof(message), "Player %c's turn", player); // Set the message
            gtk_label_set_text(GTK_LABEL(status_label), message); // Set the text of the status_label widget
        }
    }
}

void back_to_main(GtkWidget *widget, gpointer data) {
    g_print("Back Selected\n");
    execl("./Main_GUI", "./Main_GUI", (char *)NULL); // Launch the Main_GUI application
    perror("execl failed"); // Print an error message if execl fails
    exit(1); // Exit if execl fails
    //system("./Main_GUI");
    //gtk_main_quit();
}

void activate(GtkApplication *app, gpointer user_data) {
    game_data = g_malloc(sizeof(GameData)); // Allocate memory for game_data
    GtkWidget *window; // Declare a GtkWidget pointer called window
    GtkWidget *grid; // Declare a GtkWidget pointer called grid
    GtkWidget *vbox; // Declare a GtkWidget pointer called vbox
    GtkWidget *hbox; // Declare a GtkWidget pointer called hbox
 
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tic-Tac-Toe"); //title
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 1000); // Set the default size of the window to 600x1000 pixels

    // Load CSS
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // Create a vertical box
    gtk_container_add(GTK_CONTAINER(window), vbox); // Add the vertical box to the window

    gtk_widget_set_margin_top(vbox, 50); // add padding to the top of the vbox to create space above the label

    status_label = gtk_label_new("Player X's turn");
    gtk_widget_set_halign(status_label, GTK_ALIGN_CENTER); // Center the label horizontally
    gtk_box_pack_start(GTK_BOX(vbox), status_label, FALSE, FALSE, 0);

    grid = gtk_grid_new(); 
    gtk_widget_set_name(grid, "tic-tac-toe-grid"); // Set the name of the grid widget
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER); // Center the grid horizontally
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER); // Center the grid vertically
    gtk_grid_set_row_spacing(GTK_GRID(grid), 25); // Set the row spacing to 25 pixels
    gtk_grid_set_column_spacing(GTK_GRID(grid), 25); // Set the column spacing to 25 pixels
    gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 0);

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

    // Scoreboard
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // Create a horizontal box
    // gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

    // X Scoreboard
    GtkWidget *score_x_label = gtk_label_new("PLAYER X\n         0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-x");
    gtk_widget_set_size_request(score_x_label, 107, 87); // Set the size of the label
    //gtk_widget_set_margin_start(score_x_label, 10); // Add margin to the start of the label
    //gtk_widget_set_margin_end(score_x_label, 10); // Add margin to the end of the label
    //gtk_widget_set_margin_top(score_x_label, 10); // Add margin to the top of the label
    //gtk_widget_set_margin_bottom(score_x_label, 10); // Add margin to the bottom of the label
    gtk_widget_set_halign(score_x_label, GTK_ALIGN_CENTER); // Center the label horizontally
    gtk_widget_set_valign(score_x_label, GTK_ALIGN_CENTER); // Center the label vertically
    game_data->score_x_label = score_x_label;
    gtk_box_pack_start(GTK_BOX(hbox), score_x_label, TRUE, TRUE, 0);

    // O Scoreboard
    GtkWidget *score_o_label = gtk_label_new("PLAYER O\n         0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_o_label), "score-o");
    gtk_widget_set_size_request(score_o_label, 107, 87); // Set the size of the label
    //gtk_widget_set_margin_start(score_o_label, 10); // Add margin to the start of the label
    //gtk_widget_set_margin_end(score_o_label, 10); // Add margin to the end of the label
    //gtk_widget_set_margin_top(score_o_label, 10); // Add margin to the top of the label
    //gtk_widget_set_margin_bottom(score_o_label, 10); // Add margin to the bottom of the label
    gtk_widget_set_halign(score_o_label, GTK_ALIGN_CENTER); // Center the label horizontally
    gtk_widget_set_valign(score_o_label, GTK_ALIGN_CENTER); // Center the label vertically
    game_data->score_o_label = score_o_label;
    gtk_box_pack_start(GTK_BOX(hbox), score_o_label, TRUE, TRUE, 0);

    // Ties Scoreboard
    GtkWidget *score_ties_label = gtk_label_new("TIES\n    0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_ties_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_ties_label), "score-ties");
    gtk_widget_set_size_request(score_ties_label, 107, 87); // Set the size of the label
    //gtk_widget_set_margin_start(score_ties_label, 10); // Add margin to the start of the label
    //gtk_widget_set_margin_end(score_ties_label, 10); // Add margin to the end of the label
    //gtk_widget_set_margin_top(score_ties_label, 10); // Add margin to the top of the label
    //gtk_widget_set_margin_bottom(score_ties_label, 10); // Add margin to the bottom of the label
    gtk_widget_set_halign(score_ties_label, GTK_ALIGN_CENTER); // Center the label horizontally
    gtk_widget_set_valign(score_ties_label, GTK_ALIGN_CENTER); // Center the label vertically
    game_data->score_ties_label = score_ties_label;
    gtk_box_pack_start(GTK_BOX(hbox), score_ties_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0); // Add the horizontal box to the vertical box above the back button

    // Create a horizontal box for the back button
    GtkWidget *back_button_hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_halign(back_button_hbox, GTK_ALIGN_START); // Align the box to the start (left)
    gtk_widget_set_valign(back_button_hbox, GTK_ALIGN_END); // Align the box to the end (bottom)

    gtk_widget_set_margin_top(back_button_hbox, 30); // Adjust the value as needed
    gtk_box_pack_end(GTK_BOX(vbox), back_button_hbox, FALSE, FALSE, 0);

    back_button = gtk_button_new_with_label("<-Back"); // Create a button with the label "Back"
    gtk_widget_set_name(back_button, "back-button"); // Set the size of the button to 133x78 pixels
    gtk_widget_set_margin_start(back_button, 20); // Add margin to the left of the back button
    gtk_widget_set_margin_bottom(back_button, 20); // Add margin to the bottom of the back button
    gtk_box_pack_start(GTK_BOX(back_button_hbox), back_button, FALSE, FALSE, 0);
    g_signal_connect(back_button, "clicked", G_CALLBACK(back_to_main), NULL);

    gtk_widget_show_all(window);

}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;


    app = gtk_application_new("com.example.TTT_GUI2", G_APPLICATION_DEFAULT_FLAGS); // Create a new GTK application
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv); // Run the application
    g_object_unref(app);

    return status;
}