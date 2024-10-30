#include <stdio.h>
#include <stdbool.h>

// Initialize the board
char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};

// Function prototypes
void printBoard(); //nil
void takeTurn(char player); //jw
bool checkWin(char player); //qx
bool checkTie(); //jw
char switchPlayer(char currentPlayer); //qx
static int counter;
static bool gameQuit;

// Main game function
int main() {
    char currentPlayer = 'X';
    bool gameWon = false;
    bool gameTied = false;
    gameQuit = false;
    printf("Welcome to Tic-Tac-Toe!\n");
    counter = 0; //initialize counter
    printBoard();

    // Main game loop
    while (!gameWon && !gameTied && !gameQuit) {
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

    while (true){
        printf("Player %c, enter a position (1-9)(0 to quit): ", player);
        scanf("%s", input);
        if(input[0] == '0'){
            printf("Game ended\n");
            gameQuit = true;
            break;
        }
        // Convert input to integer and validate
        if (sscanf(input, "%d", &position) == 1) { // Check if input is a valid integer 
            position -= 1;  // Adjust to zero-indexed array
            if (position >= 0 && position < 9 && board[position] == '-') {
                board[position] = player;  // Place player's mark on the board
                counter++;
                break;
            }
            else if(position >= 0 && position < 9){
                printf("Position already taken. Please try again.\n");
            }
        }
        else{
            printf("Invalid input. Please try again.\n");
        }
        
    }

    printBoard();  // Display the updated board
}

// Function to check if a player has won
bool checkWin(char player) {
    // Winning combinations (rows, columns, diagonals)
    int winCombos[8][3] = {
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

// Function to check if the game is tied
bool checkTie() {
    if(counter == 9){ //if all the cells are filled
        return true;
    }
    return false;
    
}

// Function to switch players
char switchPlayer(char currentPlayer) {
    return currentPlayer == 'X' ? 'O' : 'X';
}
