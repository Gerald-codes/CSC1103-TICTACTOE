#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "constants.h"

extern char board[9];

// Minimax algorithm with alpha-beta pruning
// Returns the best score for the specified player
int minimax(char player, bool isMaximizing, int alpha, int beta) {
    // Base cases: check for a win or tie
    char winner = check_winner();
    if (winner=='X') return -1;
    if (winner=='O') return 1;
    if (winner=='D') return 0;
    
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