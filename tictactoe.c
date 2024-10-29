#include <stdio.h>
#include <string.h>

char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};             // Game board with 9 positions; '-' represents an empty position

//Initialize functions
void printBoard();
char* checkGameOver();
void takeTurn(char player);

// Main game loop
int main() {
    printBoard();
    char currentPlayer = 'X';
    int gameOver = 0;                                                       // 1 = game over, 0 = game in progress
    while (!gameOver) {
        takeTurn(currentPlayer);                                            // Player takes a turn; Call function to handle player's turn
        char* gameResult = checkGameOver();                                 // Check if the game is over
        if (strcmp(gameResult, "win") == 0) {                               // If gameResult return "win"
            printf("%c wins!\n", currentPlayer);                            // Print the winner
            gameOver = 1;                                                   // End the game
        } else if (strcmp(gameResult, "tie") == 0) {                        // Check if the game is a tie
            printf("It's a tie!\n");                                        // Print that it's a tie
            gameOver = 1;                                                   // End the game
        } else {
            // Switch to the other player
            currentPlayer = currentPlayer == 'X' ? 'O' : 'X';               // Switch players between 'X' and 'O'
        }
    }
    return 0;
}

// Function to print the game board
void printBoard() {
    printf("%c | %c | %c\n", board[0], board[1], board[2]);
    printf("%c | %c | %c\n", board[3], board[4], board[5]);
    printf("%c | %c | %c\n", board[6], board[7], board[8]);
}

// Function to handle a player's turn
void takeTurn(char player) {
    printf("\n%c's turn.\n", player);
    printf("Choose a position from 1-9: ");
    int position;
    scanf("%d", &position);
    position -= 1;
    while (position < 0 || position > 8 || board[position] != '-') {
        printf("Invalid input or position already taken. Choose a different position: ");
        scanf("%d", &position);
        position -= 1;
    }
    board[position] = player;
    printBoard();
}

// Function to check if the game is over
char* checkGameOver() {
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
