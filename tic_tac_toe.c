#include <gtk/gtk.h> 
#include <stdio.h> // Include the standard I/O library for file operations
#include <stdlib.h> 
#include "pages.h"
#include "backend.h"
#include <time.h>

extern GtkWidget *main_menu_window;
extern GtkWidget *difficulty_window;
GtkWidget *double_player_window;
GtkWidget *single_player_window;

// extern void load_players_name(char *player_x,char *player_o);
// extern void update_scoreboard();
// extern char check_winner();
// extern void reset_board();
// extern void cleanup_game_data();
// extern gboolean auto_reset_board(gpointer data);
// extern void button_clicked(GtkWidget *widget, gpointer data);
// extern void on_back_button_clicked(GtkWidget *widget, gpointer data);

LinearRegressionModel model;

typedef struct { // Define a struct called GameData
    GtkWidget *score_x_label; // Declare a GtkWidget pointer called score_x_label
    GtkWidget *score_o_label; // Declare a GtkWidget pointer called score_o_label
    GtkWidget *score_ties_label; // Declare a GtkWidget pointer called score_ties_label
    GdkPixbuf *x_pixbuf; // Declare a GdkPixbuf pointer called x_pixbuf
    GdkPixbuf *o_pixbuf; // Declare a GdkPixbuf pointer called o_pixbuf
} GameData; // Name the struct GameData

char player = 'X';
char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
GtkWidget *buttons[9]; // Array to store the button widgets
GtkWidget *back_button; // Declare a GtkWidget pointer called back_button
GtkWidget *status_label; // Declare a GtkWidget pointer called status_label

char player_x_label[10]; // Declare a string called player_x_label and set it to "PLAYER X"
char player_o_label[10];// Declare a string called player_o_label and set it to "PLAYER O"
char difficulty_mode[10];// Declare a string called player_o_label and set it to "PLAYER O"
int score_x = 0; // Declare an integer called score_x and set it to 0
int score_o = 0; // Declare an integer called score_o and set it to 0
int score_ties = 0; // Declare an integer called score_ties and set it to 0
int winning_indices[3] = {-1, -1, -1}; // Declare an integer array called winning_indices and set it to {-1, -1, -1}
static gboolean blink_state = FALSE;
static guint blink_timeout_id = 0;
GameData *game_data; // Declare a pointer to a GameData struct called game_data
gboolean auto_reset_pending = FALSE;
gboolean blink_winner(gpointer data);

extern GtkWidget *main_menu_window; // Declare a GtkWidget pointer called main_menu_window


void load_players_name(char *name_x, char *name_o) {
    strncpy(player_x_label, name_x, sizeof(player_x_label) - 1); // Copy name_x to player_x_label
    player_x_label[sizeof(player_x_label) - 1] = '\0'; // Ensure null termination

    strncpy(player_o_label, name_o, sizeof(player_o_label) - 1); // Copy name_o to player_o_label
    player_o_label[sizeof(player_o_label) - 1] = '\0'; // Ensure null termination
}

void update_scoreboard(){
    char score_text[50];
    snprintf(score_text, sizeof(score_text), "%s\n       %d",player_x_label, score_x); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_x_label), score_text);
    snprintf(score_text, sizeof(score_text), "%s\n        %d",player_o_label, score_o); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_o_label), score_text);
    snprintf(score_text, sizeof(score_text), "TIES\n    %d", score_ties); // Write the score to the score_text buffer
    gtk_label_set_text(GTK_LABEL(game_data->score_ties_label), score_text);
}

void reset_board(void);
void reset_board() {
    for (int i = 0; i < 9; i++) { // Loop through the board array
        board[i] = '-'; // Set each element to a space character
        gtk_button_set_label(GTK_BUTTON(buttons[i]), " "); // Set the label of the button to a space character
        GtkStyleContext *context = gtk_widget_get_style_context(buttons[i]); // Get the style context of the button
    }
    player = 'X'; 
    gtk_label_set_text(GTK_LABEL(status_label), "Player X's turn"); // Set the text of the status_label widget
    if (blink_timeout_id != 0) {
        g_source_remove(blink_timeout_id);
        blink_timeout_id = 0;
    }
}


char check_winner() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i * 3] == board[i * 3 + 1] && board[i * 3 + 1] == board[i * 3 + 2] && board[i * 3] != '-') {
            winning_indices[0] = i * 3;
            winning_indices[1] = i * 3 + 1;
            winning_indices[2] = i * 3 + 2;
            return board[i * 3];
        }
    }
    // Check columns
    for (int i = 0; i < 3; i++) { // Loop through the columns
        if (board[i] == board[i + 3] && board[i + 3] == board[i + 6] && board[i] != '-') { // Check if the columns are equal
            winning_indices[0] = i; // top cell of the column
            winning_indices[1] = i + 3; // middle cell of the column 
            winning_indices[2] = i + 6; // bottom cell of the column
            return board[i]; // Return the player
        }
    }
    // Check diagonals
    if (board[0] == board[4] && board[4] == board[8] && board[0] != '-') { // Check the diagonal from top-left to bottom-right
        winning_indices[0] = 0; // top-left cell
        winning_indices[1] = 4; // middle cell
        winning_indices[2] = 8; // bottom-right cell
        return board[0]; // Return the player
    }
    if (board[2] == board[4] && board[4] == board[6] && board[2] != '-') {
        winning_indices[0] = 2; // top-right cell
        winning_indices[1] = 4; // middle cell
        winning_indices[2] = 6; // bottom-left cell
        return board[2]; // Return the player
    }
    // Check for draw
    for (int i = 0; i < 9; i++) { 
        if (board[i] == '-') { // Check if there are any empty cells
            return '-'; // Game is still ongoing
        }
    }
    return 'D'; // Draw
}

gboolean auto_reset_board(gpointer data) {
    if (auto_reset_pending) {
        reset_board();
        // Re-enable all buttons
        for (int i = 0; i < 9; i++) {
            gtk_widget_set_sensitive(buttons[i], TRUE);
            gtk_widget_set_name(buttons[i], "grid-button");
        }
        auto_reset_pending = FALSE;
    }
    return FALSE; // Don't repeat the timer
}

// Function to blink the winning indices
gboolean blink_winner(gpointer data) {
    blink_state = !blink_state; // Toggle the blink state
    for (int i = 0; i < 3; i++) {
        if (winning_indices[i] != -1) { // Check if the winning index is valid
            if (blink_state) { //if blink_state is true
                gtk_button_set_label(GTK_BUTTON(buttons[winning_indices[i]]), " "); //set the button to be empty 
            } else { // if false 
                char label[2] = {board[winning_indices[i]], '\0'};
                gtk_button_set_label(GTK_BUTTON(buttons[winning_indices[i]]), label); //making the button to appear depend on X or O
            }
        }
    }
    return TRUE; // blink_winner will called again after a delay to create a blinking effect
}

void button_clicked(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data); // Convert the data pointer to an integer

    if (board[index] == '-') { // Check if the board at the index is empty
        board[index] = player;
        
        char player_str[2] = {player, '\0'}; // Create a string with the player character
        gtk_button_set_label(GTK_BUTTON(widget), player_str); // Set the label of the button to the player

        // Set the name of the widget
        if (player == 'X') {
            gtk_widget_set_name(widget, "button-x"); //extract button-x style from css
        } else {
            gtk_widget_set_name(widget, "button-o"); //extract button-o style from css
        }

        char winner = check_winner();
        if (winner != '-') { // Check if there is a winner
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
            
            // Disable all buttons
            for (int i = 0; i < 9; i++) {
                gtk_widget_set_sensitive(buttons[i], FALSE); // Disable the button
            }

        // Schedule the board reset after 3 seconds
            auto_reset_pending = TRUE;
            g_timeout_add_seconds(2, auto_reset_board, NULL);
            
        } else {
            // GET CPU MOVE FROM ML MODEL OR MINMAX
            if ((strcmp(difficulty_mode,"EASY")) == 0){
                int move = findBestMove(&model);
                    board[move] = 'O';
                    char player_str[2] = {'O', '\0'}; // Create a string with the player character
                    gtk_widget_set_name(buttons[move], "button-o"); //extract button-o style from css
                    gtk_button_set_label(GTK_BUTTON(buttons[move]), player_str); // Set the label of the button to the player
            }
            else if((strcmp(difficulty_mode,"MEDIUM")) == 0){
                srand(time(NULL)); // Seed the random number generator
                if (rand() % 2 == 0) { // 50% chance to make a random move
                    int move = findRandomMinMaxMove();
                    board[move] = 'O';
                    char player_str[2] = {'O', '\0'}; // Create a string with the player character
                    gtk_widget_set_name(buttons[move], "button-o"); //extract button-o style from css
                    gtk_button_set_label(GTK_BUTTON(buttons[move]), player_str); // Set the label of the button to the player
                } else {
                    int move = findBestMinMaxMove();
                    board[move] = 'O';
                    char player_str[2] = {'O', '\0'}; // Create a string with the player character
                    gtk_widget_set_name(buttons[move], "button-o"); //extract button-o style from css
                    gtk_button_set_label(GTK_BUTTON(buttons[move]), player_str); // Set the label of the button to the player
                }
            }
            else if ((strcmp(difficulty_mode,"HARD")) == 0){
                int move = findBestMinMaxMove();
                printf("move %d\n", move);
                board[move] = 'O';
                char player_str[2] = {'O', '\0'}; // Create a string with the player character
                gtk_widget_set_name(buttons[move], "button-o"); //extract button-o style from css
                gtk_button_set_label(GTK_BUTTON(buttons[move]), player_str); // Set the label of the button to the player
            }
            
            printf("FE board %s\n", board);


            char winner = check_winner();
            if (winner != '-') { // Check if there is a winner
                char message[50];
                if (winner == 'D') { // Check if the game is a draw
                    snprintf(message, sizeof(message), "It's a draw!"); 
                    score_ties++;
                } else { // If there is a winner
                    snprintf(message, sizeof(message), "CPU %c wins!", winner);
                    if (winner == 'X') { // Check if the winner is X
                        score_x++;
                    } else { // If the winner is O
                        score_o++;
                    }
                    blink_timeout_id = g_timeout_add(500, blink_winner, NULL); // Add a timeout to blink the winning indices
                }
            gtk_label_set_text(GTK_LABEL(status_label), message); // Set the text of the status_label widget
            update_scoreboard(); // Update the scoreboard
            
            // Disable all buttons
            for (int i = 0; i < 9; i++) {
                gtk_widget_set_sensitive(buttons[i], FALSE); // Disable the button
            }

        // Schedule the board reset after 3 seconds
            auto_reset_pending = TRUE;
            g_timeout_add_seconds(2, auto_reset_board, NULL);}
            // g_signal_connect(, "clicked", G_CALLBACK(button_clicked), GINT_TO_POINTER(i)); // Connect the "clicked" signal of the button to the button_clicked function
        }
    }
}

void button_clicked2(GtkWidget *widget, gpointer data) {
    int index = GPOINTER_TO_INT(data); // Convert the data pointer to an integer
    if (board[index] == '-') { // Check if the board at the index is empty
        board[index] = player;
        
        char player_str[2] = {player, '\0'}; // Create a string with the player character
        gtk_button_set_label(GTK_BUTTON(widget), player_str); // Set the label of the button to the player

        // Set the name of the widget
        if (player == 'X') {
            gtk_widget_set_name(widget, "button-x"); //extract button-x style from css
        } else {
            gtk_widget_set_name(widget, "button-o"); //extract button-o style from css
        }

        char winner = check_winner();
        if (winner != '-') { // Check if there is a winner
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
            
            // Disable all buttons
            for (int i = 0; i < 9; i++) {
                gtk_widget_set_sensitive(buttons[i], FALSE); // Disable the button
            }

        // Schedule the board reset after 3 seconds
            auto_reset_pending = TRUE;
            g_timeout_add_seconds(2, auto_reset_board, NULL);
            
        } else {
            player = (player == 'X') ? 'O' : 'X'; // Switch players

            char message[50]; 
            snprintf(message, sizeof(message), "Player %c's turn", player); // Set the message
            gtk_label_set_text(GTK_LABEL(status_label), message); // Set the text of the status_label widget
        }
    }
}

void cleanup_game_data() {
    if (game_data != NULL) {
        free(game_data);
        game_data = NULL;
    }
}

// Callback function for the "Back" button
void on_back_button_clicked(GtkWidget *widget, gpointer data) {
    // Reset scores
    score_x = 0;
    score_o = 0;
    score_ties = 0;

    // Update the scoreboard
    update_scoreboard();

    // Reset the board
    reset_board();
    
    // Clean up the game data
    cleanup_game_data();

    // Hide the double player window
    gtk_widget_hide(GTK_WIDGET(data));

    // Show the main menu window
    gtk_widget_show_all(main_menu_window);
}


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
        g_signal_connect(buttons[i], "clicked", G_CALLBACK(button_clicked2), GINT_TO_POINTER(i)); // Connect the "clicked" signal of the button to the button_clicked function
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

}

void show_single_player_page(GtkWidget *difficulty_window, char *difficulty) {
    if (strcmp(difficulty,"EASY")){
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
        parseDataset(&model);  // Parse the dataset

        // // Print bot's weights
        // printf("Bot's weights: ");
        // for (int i = 0; i < BOARD_SIZE; i++) {
        //     printf("%f ", model.weights[i]);
        // }
        // printf("\n");
    }else if(strcmp(difficulty, "MEDIUM")){
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
    }else{
        strncpy(difficulty_mode, difficulty, sizeof(difficulty_mode) - 1); // Copy name_x to player_x_label
        difficulty_mode[sizeof(difficulty_mode) - 1] = '\0'; // Ensure null termination
    }
    printf("Difficulty: %s\n", difficulty_mode);

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
    gtk_window_set_default_size(GTK_WINDOW(single_player_window), 600, 1000);
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
    gtk_fixed_put(GTK_FIXED(fixed), mode, 0, 50);

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
    GtkWidget *score_x_label = gtk_label_new("X (YOU)\n       0");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-box");
    gtk_style_context_add_class(gtk_widget_get_style_context(score_x_label), "score-x");
    gtk_widget_set_size_request(score_x_label, 107, 87); // Set the size of the label
    game_data->score_x_label = score_x_label;
    gtk_fixed_put(GTK_FIXED(fixed), score_x_label, 50, 650);


    // O Scoreboard
    GtkWidget *score_o_label = gtk_label_new("O (CPU)\n        0");
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
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_button_clicked), single_player_window);
    gtk_button_set_image(GTK_BUTTON(back_button),arrow);
    gtk_fixed_put(GTK_FIXED(fixed), back_button, 10, 800);

    // Show all widgets in the single player window
    gtk_widget_show_all(single_player_window);

    // Connect the delete-event signal to quit the application when the single player window is closed
    g_signal_connect(single_player_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

}