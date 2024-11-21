#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "linearRegression.c" // Include the linear regression model
#include "pages.h" // Include page-related definitions

// Define constants for dataset processing
#define LINE_BUFFER_SIZE 1024 // Maximum size of a line in the dataset
#define ROWS 958 // Total number of rows in the dataset
#define COLUMNS 10 // Number of columns in the dataset
#define MAX_ARRAY_SIZE 100 // Maximum size of array elements
#define BOARD_SIZE 9 // Size of the Tic-Tac-Toe board (3x3)

// Function Declarations
void convert_and_read_file(const char *filename, char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]);
void split_dataset(char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char training_set[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testing_set[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]);
void shuffle_dataset(char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]);
extern char board[9]; // Reference the global board

// Function to shuffle rows in the Tic-Tac-Toe dataset
void shuffle_dataset(char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]) {
    srand(time(NULL)); // Seed the random number generator

    char temp_row[COLUMNS][MAX_ARRAY_SIZE]; // Temporary buffer to hold a row during swapping

    for (int i = ROWS - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Generate a random index between 0 and i

        // Swap rows i and j
        memcpy(temp_row, tictactoe_dataset[i], sizeof(temp_row)); // Copy row i to temp
        memcpy(tictactoe_dataset[i], tictactoe_dataset[j], sizeof(temp_row)); // Copy row j to row i
        memcpy(tictactoe_dataset[j], temp_row, sizeof(temp_row)); // Copy temp back to row j
    }
}

// Function to read and process the dataset file
void convert_and_read_file(const char *filename, char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]) {
    FILE *dataset = fopen(filename, "r");
    if (dataset == NULL) {
        printf("Error opening file\n");
        exit(1); // Exit the program if the file cannot be opened
    }

    char line[LINE_BUFFER_SIZE]; // Buffer to hold each line of the dataset
    int row = 0;

    // Read the file line by line
    while (fgets(line, sizeof(line), dataset) && row < ROWS) {
        line[strcspn(line, "\n")] = 0; // Remove the newline character

        int col = 0;
        char *token = strtok(line, ","); // Split the line into tokens
        while (token != NULL && col < COLUMNS) {
            // Convert token values to corresponding strings
            if (strcmp(token, "x") == 0) {
                strcpy(tictactoe_dataset[row][col], "1"); // 'x' is represented as 1
            } else if (strcmp(token, "o") == 0) {
                strcpy(tictactoe_dataset[row][col], "-1"); // 'o' is represented as -1
            } else if (strcmp(token, "b") == 0) {
                strcpy(tictactoe_dataset[row][col], "0"); // 'b' is represented as 0
            } else if (strcmp(token, "positive") == 0) {
                strcpy(tictactoe_dataset[row][col], "1"); // 'positive' outcome is 1
            } else if (strcmp(token, "negative") == 0) {
                strcpy(tictactoe_dataset[row][col], "0"); // 'negative' outcome is 0
            }
            token = strtok(NULL, ","); // Move to the next token
            col++;
        }
        row++;
    }
    fclose(dataset); // Close the file after reading

    // Shuffle the dataset for randomness
    shuffle_dataset(tictactoe_dataset);
}

// Function to split the dataset into training and testing sets
void split_dataset(char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char training_set[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testing_set[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]) {
    int training_index = 0; // Index for the training set
    int testing_index = 0; // Index for the testing set

    for (int i = 0; i < ROWS; i++) {
        if (i < ROWS * 0.8) { // Allocate 80% of the data for training
            for (int j = 0; j < COLUMNS; j++) {
                strcpy(training_set[training_index][j], tictactoe_dataset[i][j]);
            }
            training_index++;
        } else { // Allocate 20% of the data for testing
            for (int j = 0; j < COLUMNS; j++) {
                strcpy(testing_set[testing_index][j], tictactoe_dataset[i][j]);
            }
            testing_index++;
        }
    }
}

// Bot chooses the best move based on the linear regression model
int find_best_move(LinearRegressionModel *model) {
    int best_move = -1; // Index of the best move
    double best_score = -1e9; // Initialize best score to a very low value

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (board[i] == '-') { // Check if the cell is empty
            int temp_board[BOARD_SIZE];
            for (int j = 0; j < BOARD_SIZE; j++) {
                // Convert the board to numeric representation
                temp_board[j] = (board[j] == 'X') ? 1 : (board[j] == 'O') ? -1 : 0;
            }
            temp_board[i] = -1; // Simulate the bot placing 'O' in the cell

            double score = predict(model, temp_board); // Predict the score for the move
            printf("Move %d Score: %f\n", i + 1, score); // Print the score for debugging
            if (score > best_score) { // Update the best score and move
                best_score = score;
                best_move = i;
            }
        }
    }
    return best_move; // Return the index of the best move
}

// Parse and train the dataset
int parse_dataset(LinearRegressionModel *model) {
    char tictactoe_dataset[ROWS][COLUMNS][MAX_ARRAY_SIZE];
    const char *filename = "tic-tac-toe.data"; // Dataset file

    // Read the dataset and shuffle it
    convert_and_read_file(filename, tictactoe_dataset);

    // Split the dataset into training and testing sets
    char training_set[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE];
    char testing_set[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE];
    split_dataset(tictactoe_dataset, training_set, testing_set);

    // Train the linear regression model
    create_model(model, training_set, testing_set);

    // Print the model's weights for debugging
    printf("Bot's weights: ");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%f ", model->weights[i]);
    }
    printf("\n");

    return 0; // Return success
}
