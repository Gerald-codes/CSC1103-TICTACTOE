#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linearRegression.c"

#define LINE_BUFFER_SIZE 1024
#define ROWS 958
#define COLUMNS 10
#define MAX_ARRAY_SIZE 100
#define BOARD_SIZE 9

void convertAndReadFile(const char *filename, char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]);
void splitDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char trainingSet[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testingSet[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]);
void shuffleDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]);

// Function to shuffle rows in tictactoeDataset
void shuffleDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]) {
    srand(time(NULL)); // Seed the random number generator

    // Temporary buffer to hold a row during swapping
    char tempRow[COLUMNS][MAX_ARRAY_SIZE];

    for (int i = ROWS - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Random index between 0 and i

        // Swap rows i and j
        memcpy(tempRow, tictactoeDataset[i], sizeof(tempRow));             // Copy row i to temp
        memcpy(tictactoeDataset[i], tictactoeDataset[j], sizeof(tempRow)); // Copy row j to row i
        memcpy(tictactoeDataset[j], tempRow, sizeof(tempRow));             // Copy temp to row j
    }
}


// Function to read the file
void convertAndReadFile(const char *filename, char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]) {
    FILE *dataset = fopen(filename, "r");
    if (dataset == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    char line[LINE_BUFFER_SIZE];
    int row = 0;

    while (fgets(line, sizeof(line), dataset) && row < ROWS) {
        // Remove the newline character
        line[strcspn(line, "\n")] = 0;

        int col = 0;
        char *token = strtok(line, ",");
        while (token != NULL && col < COLUMNS) {
            // Convert tokens to corresponding strings
            if (strcmp(token, "x") == 0) {
                strcpy(tictactoeDataset[row][col], "1");
            } else if (strcmp(token, "o") == 0) {
                strcpy(tictactoeDataset[row][col], "-1");
            } else if (strcmp(token, "b") == 0) {
                strcpy(tictactoeDataset[row][col], "0");
            } else if (strcmp(token, "positive") == 0) {
                strcpy(tictactoeDataset[row][col], "1");
            } else if (strcmp(token, "negative") == 0) {
                strcpy(tictactoeDataset[row][col], "0");
            }
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(dataset);

    // Shuffle the dataset
    shuffleDataset(tictactoeDataset);
}

// Split the dataset into training and testing sets
void splitDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char trainingSet[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testingSet[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]) {
    int trainingIndex = 0;
    int testingIndex = 0;
    for (int i = 0; i < ROWS; i++) {
        if (i < ROWS * 0.8) {
            for (int j = 0; j < COLUMNS; j++) {
                strcpy(trainingSet[trainingIndex][j], tictactoeDataset[i][j]);
            }
            trainingIndex++;
        } else {
            for (int j = 0; j < COLUMNS; j++) {
                strcpy(testingSet[testingIndex][j], tictactoeDataset[i][j]);
            }
            testingIndex++;
        }
    }
}
//CREATE BOARD TO TRY HERE

// Global board for the game
char board[BOARD_SIZE] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};

// Function to print the game board
void printBoard() {
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%c ", board[i]);
        if ((i + 1) % 3 == 0) printf("\n");
    }
}

// Function to check if there's a winner
int checkWinner() {
    int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // Rows
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // Columns
        {0, 4, 8}, {2, 4, 6}              // Diagonals
    };

    for (int i = 0; i < 8; i++) {
        int a = winPatterns[i][0], b = winPatterns[i][1], c = winPatterns[i][2];
        if (board[a] == board[b] && board[b] == board[c] && board[a] != '-') {
            return board[a] == 'X' ? 1 : -1;  // 1 for player win, -1 for bot win
        }
    }
    return 0;  // No winner yet
}

// Check if the board is full
int isBoardFull() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == '-') return 0;  // Empty space found
    }
    return 1;  // No empty spaces
}
// Function for the player to take a turn
void playerTurn() {
    int position;
    printf("\nYour move! Choose a position (1-9): ");
    scanf("%d", &position);
    position -= 1;

    while (position < 0 || position >= BOARD_SIZE || board[position] != '-') {
        printf("Invalid move. Choose another position (1-9): ");
        scanf("%d", &position);
        position -= 1;
    }
    board[position] = 'X';  // Player move
}
// Bot chooses the best move using the linear regression model
int findBestMove(LinearRegressionModel *model) {
    int bestMove = -1;
    double bestScore = -1e9;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == '-') {  // Empty cell
            int tempBoard[BOARD_SIZE];
            for (int j = 0; j < BOARD_SIZE; j++) {
                tempBoard[j] = (board[j] == 'X') ? 1 : (board[j] == 'O') ? -1 : 0;
            }
            tempBoard[i] = -1;  // Simulate bot placing 'O'

            double score = predict(model, tempBoard);
            printf("Move %d Score: %f\n", i, score);
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    return bestMove;
}
// Function for the bot to take a turn
void botTurn(LinearRegressionModel *model) {
    printf("\nBot is thinking...\n");
    int move = findBestMove(model);
    if (move != -1) {
        board[move] = 'O';  // Bot move
    }
}
// Main game loop
void playGame(LinearRegressionModel *model) {
    int winner = 0;

    while (1) {
        printBoard();
        playerTurn();
        winner = checkWinner();
        if (winner || isBoardFull()) break;

        botTurn(model);
        winner = checkWinner();
        if (winner || isBoardFull()) break;
    }

    printBoard();
    if (winner == 1) {
        printf("Congratulations! You win!\n");
    } else if (winner == -1) {
        printf("Bot wins! Better luck next time!\n");
    } else {
        printf("It's a draw!\n");
    }
}

// Other game-related functions remain unchanged...

int main() {
    char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE];
    const char *filename = "tic-tac-toe.data";

    // Read the dataset and shuffle it
    convertAndReadFile(filename, tictactoeDataset);

    // Split the dataset into training and testing sets
    char trainingSet[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE];
    char testingSet[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE];
    splitDataset(tictactoeDataset, trainingSet, testingSet);

    LinearRegressionModel model;
    createModel(&model, trainingSet, testingSet);

    // Print bot's weights
    printf("Bot's weights: ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%f ", model.weights[i]);
    }
    printf("\n");

    // Play the game
    playGame(&model);

    return 0;
}
