#include <gtk/gtk.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> 
#include "constants.h"


GtkWidget *main_menu_window = NULL;
GtkWidget *difficulty_window = NULL;
GtkWidget *buttons[9] = {NULL};
GtkWidget *back_button = NULL;
GtkWidget *end_button = NULL;
GtkWidget *status_label = NULL;

char player = 'X';
char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char player_x_label[10] = "PLAYER X";
char player_o_label[10] = "PLAYER O";

int score_x = 0;
int score_o = 0;
int score_ties = 0;
int winning_indices[3] = {-1, -1, -1};

gboolean auto_reset_pending = FALSE;
guint blink_timeout_id = 0;
gboolean blink_state = FALSE;
GameData *game_data = NULL;

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
            player = (player == 'X') ? 'O' : 'X'; // Switch players
            char message[50]; 
            snprintf(message, sizeof(message), "Player %c's turn", player); // Set the message
            gtk_label_set_text(GTK_LABEL(status_label), message); // Set the text of the status_label widget
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



bool checkWin(char player) {
    // Winning combinations (rows, columns, diagonals)
    int winCombos[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    // Check if any winning combination is met
    for (int i = 0; i < 8; i++) { //each winning combination in winCombos array
        if (board[winCombos[i][0]] == player && // check if all 3 cells in the contained in the combination contain the the specified player's symbol(X or O)
            board[winCombos[i][1]] == player &&
            board[winCombos[i][2]] == player) {
            winning_indices[0] = winCombos[i][0]; // 1st cell of the i-the winning combination in the array, the value is assigned to winning_indices[0] 
            winning_indices[1] = winCombos[i][1]; // 2nd cell 
            winning_indices[2] = winCombos[i][2]; // 3rd cell
            return true;
        }
    }
    return false;
}
bool checkTie() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == ' ') {
            return false; // There is at least one empty cell, so it's not a tie
        }
    }
    return true; // All cells are filled, so it's a tie
}

