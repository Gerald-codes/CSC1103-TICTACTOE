#include <stdio.h>
#include <stdbool.h>

// Initialize the board
char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};

// Function prototypes
void printBoard();
void takeTurn(char player);
bool checkWin(char player);
bool checkTie();
char switchPlayer(char currentPlayer);

// Main game function
int main() {
    char currentPlayer = 'X';
    bool gameWon = false;
    bool gameTied = false;

    printf("Welcome to Tic-Tac-Toe!\n");
    printBoard();

    // Main game loop
    while (!gameWon && !gameTied) {
        takeTurn(currentPlayer);  // Player takes turn
        gameWon = checkWin(currentPlayer);  // Check if the current player won
        gameTied = checkTie();  // Check if the game is tied

        if (gameWon) {
            printf("Player %c wins!\n", currentPlayer);  // Announce the winner
        } else if (gameTied) {
            printf("It's a tie!\n");  // Announce a tie
        } else {
            currentPlayer = switchPlayer(currentPlayer);  // Switch players
        }
    }

    return 0;
}

// Function to print the current state of the board
void printBoard() {
    printf("\n");
    for (int i = 0; i < 9; i++) {
        printf("%c ", board[i]);
        if (i % 3 == 2) {
            printf("\n");  // Print a new line after every 3 positions
        }
    }
    printf("\n");
}

// Function to handle a player's turn
void takeTurn(char player) {
    int position = -1;
    char input[10];

    while (1) {
        printf("Player %c, enter a position (1-9): ", player);
        scanf("%s", input);

        // Convert input to integer and validate
        if (sscanf(input, "%d", &position) == 1) {
            position -= 1;  // Adjust to zero-indexed array
            if (position >= 0 && position < 9 && board[position] == '-') {
                board[position] = player;  // Place player's mark on the board
                break;
            }
        }
        printf("Invalid input or position already taken. Please try again.\n");
    }

    printBoard();  // Display the updated board
}

// Function to check if a player has won
bool checkWin(char player) {
    // Winning combinations (rows, columns, diagonals)
    int winCombos[8][3] = {               // 8 winning combinations
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    // Check if any winning combination is met
    for (int i = 0; i < 8; i++) {
        if (board[winCombos[i][0]] == player &&
            board[winCombos[i][1]] == player &&
            board[winCombos[i][2]] == player) {
            return true;
        }
    }
    return false;
}

// Function to check if the game is tied. 
bool checkTie() {
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            return false;       // If there's an empty spot, it's not a tie
        }
    }
    return true;                // If all spots are filled and no one won, it's a tie
}

// Function to switch players. If player is 'X', switch to 'O' and vice versa
char switchPlayer(char currentPlayer) {
    if (currentPlayer == 'X')
    {
        return 'O';
    }
    else
    {
        return 'X';
    }
}
