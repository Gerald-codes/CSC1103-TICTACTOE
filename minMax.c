#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

extern char board[9];

// Function to check if the game is over
const char* check_game_over() {
    // Check for a win
    if ((board[0] == board[1] && board[1] == board[2] && board[0] != '-') ||
        (board[3] == board[4] && board[4] == board[5] && board[3] != '-') ||
        (board[6] == board[7] && board[7] == board[8] && board[6] != '-') ||
        (board[0] == board[3] && board[3] == board[6] && board[0] != '-') ||
        (board[1] == board[4] && board[4] == board[7] && board[1] != '-') ||
        (board[2] == board[5] && board[5] == board[8] && board[2] != '-') ||
        (board[0] == board[4] && board[4] == board[8] && board[0] != '-') ||
        (board[2] == board[4] && board[4] == board[6] && board[2] != '-')) {
        return "win";
    }
    // Check for a tie
    int is_tie = 1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            is_tie = 0;
            break;
        }
    }
    if (is_tie) {
        return "tie";
    }
    // Game is not over
    return "play";
}

// Function to check if a player has won
bool check_player_win(char player) {
    return ((board[0] == player && board[1] == player && board[2] == player) ||
            (board[3] == player && board[4] == player && board[5] == player) ||
            (board[6] == player && board[7] == player && board[8] == player) ||
            (board[0] == player && board[3] == player && board[6] == player) ||
            (board[1] == player && board[4] == player && board[7] == player) ||
            (board[2] == player && board[5] == player && board[8] == player) ||
            (board[0] == player && board[4] == player && board[8] == player) ||
            (board[2] == player && board[4] == player && board[6] == player));
}

// Minimax algorithm with alpha-beta pruning
// Returns the best score for the specified player
int minimax(char player, bool isMaximizing, int alpha, int beta) {
    // Base cases: check for a win or tie
    if (check_player_win('X')) return -1; // 'X' wins
    if (check_player_win('O')) return 1;  // 'O' wins
    if (strcmp(check_game_over(), "tie") == 0) return 0; // Tie
    if (isMaximizing) {
        int max_eval = -1000; // Initialize max_eval to a very low value for maximizing player
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') { // Check if the position is empty
                board[i] = player; // Simulate move
                int eval = minimax(player == 'O' ? 'X' : 'O', false, alpha, beta); // Recursive call for minimizing player
                board[i] = '-'; // Undo move (backtrack)
                max_eval = eval > max_eval ? eval : max_eval; // Update max_eval with the maximum value
                alpha = alpha > eval ? alpha : eval; // Update alpha with the maximum value
                if (beta <= alpha) { // Beta cutoff
                    break; // Prune the remaining branches
                }
            }
        }
        return max_eval; // Return the best score for the maximizing player
    } else {
        int min_eval = 1000; // Initialize min_eval to a very high value for minimizing player
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') { // Check if the position is empty
                board[i] = player; // Simulate move
                int eval = minimax(player == 'O' ? 'X' : 'O', true, alpha, beta); // Recursive call for maximizing player
                board[i] = '-'; // Undo move (backtrack)
                min_eval = eval < min_eval ? eval : min_eval; // Update min_eval with the minimum value
                beta = beta < eval ? beta : eval; // Update beta with the minimum value
                if (beta <= alpha) { // Alpha cutoff
                    break; // Prune the remaining branches
                }
            }
        }
        return min_eval; // Return the best score for the minimizing player
    }
}

// Function to find the best move for the computer
// Returns the index of the best move for the computer
int find_best_min_max_move() {
    int best_move = -1;
    int best_score = -1000;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            board[i] = 'O';
            int score = minimax('X', false, -1000, 1000);
            board[i] = '-';
            if (score > best_score) {
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move;
}

// Function to find a random move for the computer to lower difficulty level, so the computer doesn't always make the best move
// Returns the index of a random available move for the computer
int find_random_min_max_move() {
    int available_moves[9];
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            available_moves[count++] = i;
        }
    }
    if (count > 0) {
        return available_moves[rand() % count];
    }
    return -1;
}