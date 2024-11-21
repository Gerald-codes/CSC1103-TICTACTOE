#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

extern char board[9];

// Function to check if the game is over
const char* checkGameOver() {
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
    int isTie = 1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            isTie = 0;
            break;
        }
    }
    if (isTie) {
        return "tie";
    }
    // Game is not over
    return "play";
}

// Function to check if a player has won
bool checkPlayerWin(char player) {
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
    if (checkPlayerWin('X')) return -1; // 'X' wins
    if (checkPlayerWin('O')) return 1;  // 'O' wins
    if (strcmp(checkGameOver(), "tie") == 0) return 0; // Tie
    if (isMaximizing) {
        int maxEval = -1000; // Initialize maxEval to a very low value for maximizing player
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') { // Check if the position is empty
                board[i] = player; // Simulate move
                int eval = minimax(player == 'O' ? 'X' : 'O', false, alpha, beta); // Recursive call for minimizing player
                board[i] = '-'; // Undo move (backtrack)
                maxEval = eval > maxEval ? eval : maxEval; // Update maxEval with the maximum value
                alpha = alpha > eval ? alpha : eval; // Update alpha with the maximum value
                if (beta <= alpha) { // Beta cutoff
                    break; // Prune the remaining branches
                }
            }
        }
        return maxEval; // Return the best score for the maximizing player
    } else {
        int minEval = 1000; // Initialize minEval to a very high value for minimizing player
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') { // Check if the position is empty
                board[i] = player; // Simulate move
                int eval = minimax(player == 'O' ? 'X' : 'O', true, alpha, beta); // Recursive call for maximizing player
                board[i] = '-'; // Undo move (backtrack)
                minEval = eval < minEval ? eval : minEval; // Update minEval with the minimum value
                beta = beta < eval ? beta : eval; // Update beta with the minimum value
                if (beta <= alpha) { // Alpha cutoff
                    break; // Prune the remaining branches
                }
            }
        }
        return minEval; // Return the best score for the minimizing player
    }
}

// Function to find the best move for the computer
// Returns the index of the best move for the computer
int findBestMinMaxMove() {
    int bestMove = -1;
    int bestScore = -1000;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            board[i] = 'O';
            int score = minimax('X', false, -1000, 1000);
            board[i] = '-';
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }
    return bestMove;
}

// Function to find a random move for the computer to lower difficulty level, so the computer doesn't always make the best move
// Returns the index of a random available move for the computer
int findRandomMinMaxMove() {
    int availableMoves[9];
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            availableMoves[count++] = i;
        }
    }
    if (count > 0) {
        return availableMoves[rand() % count];
    }
    return -1;
}