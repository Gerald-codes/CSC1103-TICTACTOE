#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
int playerWins = 0; //variables to keep track of each player's victories
int computerWins = 0;

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
int minimax(char player, bool isMaximizing, int alpha, int beta) {
    if (checkWin('X')) return -1;
    if (checkWin('O')) return 1;
    if (strcmp(checkGameOver(), "tie") == 0) return 0;

    if (isMaximizing) {
        int maxEval = -1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') {
                board[i] = player;
                int eval = minimax(player == 'O' ? 'X' : 'O', false, alpha, beta);
                board[i] = '-';
                maxEval = eval > maxEval ? eval : maxEval;
                alpha = alpha > eval ? alpha : eval;
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return maxEval;
    } else {
        int minEval = 1000;
        for (int i = 0; i < 9; i++) {
            if (board[i] == '-') {
                board[i] = player;
                int eval = minimax(player == 'O' ? 'X' : 'O', true, alpha, beta);
                board[i] = '-';
                minEval = eval < minEval ? eval : minEval;
                beta = beta < eval ? beta : eval;
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return minEval;
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

// Function to find a random move for the computer
int findRandomMove() {
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

// Function to handle a player's turn
void takeTurn(char player) {
    if (player == 'O') {
        int move;
        if (rand() % 2 == 0) { // 50% chance to make a random move
            move = findRandomMove();
        } else {
            move = findBestMove();
        }
        board[move] = 'O';
    } else {
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
    }
      if(player == 'O') 
      {
        printBoard();
      }
           
}

// Main game loop
int main() {
    srand(time(NULL));
    char playAgain;
    do {
        // Reset board
        memset(board, '-', sizeof(board));
        char currentPlayer = 'X';
        int gameOver = 0;
        printBoard();

        while (!gameOver) {
            takeTurn(currentPlayer);
            const char* gameResult = checkGameOver();
            if (strcmp(gameResult, "win") == 0) 
            {
                if(currentPlayer == 'X')
                {
                    printBoard();
                }
                printf("%c wins!\n", currentPlayer);
                currentPlayer == 'X'? playerWins++ : computerWins++;
                gameOver = 1;
            }
             else if (strcmp(gameResult, "tie") == 0) {
                printBoard();
                printf("It's a tie!\n");
                gameOver = 1;
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        }

        printf("Number of times Player has won: %d\n", playerWins);
        printf("Number of times Computer has won: %d\n", computerWins);
        printf("Do you want to play again? (y/n): ");
        scanf(" %c", &playAgain);
        while (getchar() != '\n'); // Clear the input buffer
    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}