#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
int playerWins = 0; // Variables to keep track of each player's victories and ties
int computerWins = 0;
int ties = 0;

// Function to print the game board
void printBoard() {
    printf("%c | %c | %c\n", board[0], board[1], board[2]);
    printf("%c | %c | %c\n", board[3], board[4], board[5]);
    printf("%c | %c | %c\n", board[6], board[7], board[8]);
}

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
bool checkWin(char player) {
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
    if (checkWin('X')) return -1; // 'X' wins
    if (checkWin('O')) return 1;  // 'O' wins
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
int findBestMove() {
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

// Function to handle a player's turn
void takeTurn(char player) 
{
    if (player == 'O') 
    {
        int move = findBestMove();
        board[move] = 'O';
    } 
    else 
    {
        printf("\n%c's turn.\n", player);
        printf("Choose a position from 1-9: ");
        int position;
        while (scanf("%d", &position) != 1 || position < 1 || position > 9 || board[position - 1] != '-') 
        {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input or position already taken. Choose a different position: ");
        }
        position -= 1;
        board[position] = player;

    }
    if(player == 'O')
    {
        printBoard(); // Printing the board only after the computer has made its move to avoid double printing

    }
   
}

char promptPlayAgain() {
    char playAgain;
    do {
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &playAgain);
        while (getchar() != '\n'); // Clear the input buffer
    } while (playAgain != 'y' && playAgain != 'n' && playAgain != 'Y' && playAgain != 'N'); // Repeat if the input is invalid
    return playAgain;
}



void runGame()
{
    char playAgain;
    do {
        // Reset board
        memset(board, '-', sizeof(board)); // Initialize the board to an empty state
        char currentPlayer = 'X'; // Start with player 'X'
        int gameOver = 0; // Flag to check if the game is over
        printBoard(); // Print the initial empty board

        while (!gameOver) {
            takeTurn(currentPlayer); // Handle the current player's turn
            const char* gameResult = checkGameOver(); // Check if the game is over
            if (strcmp(gameResult, "win") == 0) 
            { // If there is a winner
                if (currentPlayer == 'X') 
                {
                    printBoard(); // Print the board one last time if 'X' wins
                }
                printf("%c wins!\n", currentPlayer); // Announce the winner
                currentPlayer == 'X' ? playerWins++ : computerWins++; // Update win count
                gameOver = 1; // Set game over flag
            } 
            else if (strcmp(gameResult, "tie") == 0) { // If the game is a tie
                ties++; // Increment the tie count
                printBoard(); // Print the board one last time
                printf("It's a tie!\n"); // Announce the tie
                gameOver = 1; // Set game over flag
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Switch to the other player
            }
        }

        // Print the number of wins for each player and ties
        printf("Number of times Player has won: %d\n", playerWins);
        printf("Number of times Computer has won: %d\n", computerWins);
        printf("Number of ties: %d\n", ties);
        // Prompt the user to play again and validate the input
        playAgain = promptPlayAgain();
        
    } while (playAgain == 'y' || playAgain == 'Y'); // Repeat the game loop if the players want to play again

}

// Main game loop
int main() {
    runGame();
    return 0;
}