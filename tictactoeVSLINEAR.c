#include <stdio.h>
#include <string.h>
#include "parseDataset.c"
#include "linearRegression.c"
#define BOARD_SIZE 9  // 3x3 Tic-Tac-Toe board
#define LEARNING_RATE 0.01
#define EPOCHS 1000
#define MAX_SAMPLES 958
#define TRAINING_SIZE (int)(958 * 0.8)
#define TESTING_SIZE (int)(958 * 0.2)
#define COLUMNS 10
#define MAX_ARRAY_SIZE 100

#define LINE_BUFFER_SIZE 1024
#define ROWS 958
#define COLUMNS 10
#define MAX_ARRAY_SIZE 100

char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};

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

// Main game loop
int main() {

    char currentPlayer = 'X';
    int gameOver = 0;
    while (!gameOver) {
        takeTurn(currentPlayer);
        char* gameResult = checkGameOver();
        if (strcmp(gameResult, "win") == 0) {
            printf("%c wins!\n", currentPlayer);
            gameOver = 1;
        } else if (strcmp(gameResult, "tie") == 0) {
            printf("It's a tie!\n");
            gameOver = 1;
        } else {
            // Switch to the other player
            currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
        }
    }
    return 0;
}
