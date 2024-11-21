#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"

// Constants for the model and dataset
#define BOARD_SIZE 9  // Number of features for each board (3x3 Tic-Tac-Toe)
#define LEARNING_RATE 0.0001  // Learning rate for gradient descent
#define EPOCHS 958  // Number of epochs for training
#define MAX_SAMPLES 958  // Maximum number of data samples
#define TRAINING_SIZE (int)(958 * 0.8)  // 80% of the data for training
#define TESTING_SIZE (int)(958 * 0.2)  // 20% of the data for testing
#define COLUMNS 10  // Number of columns in the dataset
#define MAX_ARRAY_SIZE 100  // Maximum size for array elements

// Function Declarations
void initialize_model(LinearRegressionModel *model); // Initialize model weights and bias
double predict(LinearRegressionModel *model, int board[]); // Predict outcome for a given board
void train(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]); // Train the model
double evaluate_testing(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]); // Evaluate model on testing data
double evaluate_training(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]); // Evaluate model on training data
int create_model(LinearRegressionModel *model, char training_set[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testing_set[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]); // Main function to create, train, and evaluate the model
void parse_lr_dataset(char dataset[][COLUMNS][MAX_ARRAY_SIZE], int boards[][BOARD_SIZE], double outcomes[], int size); // Parse dataset from strings to integers/doubles

// Function to initialize the model with random weights and bias
void initialize_model(LinearRegressionModel *model) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        model->weights[i] = (double) rand() / RAND_MAX;  // Random weight initialization
    }
    model->bias = (double) rand() / RAND_MAX;  // Random bias initialization
}

// Predict the outcome for a given board state using the model
double predict(LinearRegressionModel *model, int board[]) {
    double score = model->bias;  // Start with the bias
    for (int i = 0; i < BOARD_SIZE; i++) {
        score += model->weights[i] * board[i];  // Add weighted contributions from each board feature
    }
    return score;  // Return the predicted score
}

// Train the model using gradient descent
void train(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0;  // Accumulate mean squared error for the epoch
        for (int i = 0; i < TRAINING_SIZE; i++) {

            // Predict the outcome for the current board
            double prediction = predict(model, boards[i]);
            double error = prediction - outcomes[i];  // Calculate error

            // Update weights and bias using gradient descent
            for (int j = 0; j < BOARD_SIZE; j++) {
                model->weights[j] -= LEARNING_RATE * error * boards[i][j];  // Adjust weight
            }
            model->bias -= LEARNING_RATE * error;  // Adjust bias

            total_error += error * error;  // Accumulate squared error
        }

        // Optionally print the loss and model parameters every 100 epochs
        if (epoch % 100 == 0) {
            printf("Epoch %d - Weights: ", epoch);
            for (int k = 0; k < BOARD_SIZE; k++) {
                printf("%f ", model->weights[k]);
            }
            printf("Bias: %f\n", model->bias);
            printf("Epoch %d: Error = %f\n", epoch, total_error / TRAINING_SIZE);
        }
    }
}

// Function to evaluate the model's accuracy on testing data
double evaluate_testing(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]) {
    int correct_predictions = 0; // Count of correctly classified samples
    int true_positive = 0, true_negative = 0, false_positive = 0, false_negative = 0;

    for (int i = 0; i < TESTING_SIZE; i++) {
        double prediction = predict(model, boards[i]); // Predict outcome
        int predicted_class = (prediction >= 0.5) ? 1 : 0; // Threshold for classification

        // Update counts for accuracy and confusion matrix
        if (predicted_class == (int)outcomes[i]) {
            correct_predictions++;
        }
        if (predicted_class == 1 && (int)outcomes[i] == 1) {
            true_positive++;
        }
        if (predicted_class == 0 && (int)outcomes[i] == 0) {
            true_negative++;
        }
        if (predicted_class == 1 && (int)outcomes[i] == 0) {
            false_positive++;
        }
        if (predicted_class == 0 && (int)outcomes[i] == 1) {
            false_negative++;
        }
    }

    // Print confusion matrix
    printf("---------------------------------------------------\n");
    printf("True Positive: %d\t | False Positive: %d\n", true_positive, false_positive);
    printf("True Negative: %d\t | False Negative: %d\n", true_negative, false_negative);
    printf("----------------------------------------------------\n");

    return (double) correct_predictions / TESTING_SIZE; // Return accuracy
}

// Function to evaluate the model's accuracy on training data
double evaluate_training(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]) {
    int correct_predictions = 0; // Count of correctly classified samples
    int true_positive = 0, true_negative = 0, false_positive = 0, false_negative = 0;

    for (int i = 0; i < TRAINING_SIZE; i++) {
        double prediction = predict(model, boards[i]); // Predict outcome
        int predicted_class = (prediction >= 0.5) ? 1 : 0; // Threshold for classification

        // Update counts for accuracy and confusion matrix
        if (predicted_class == (int)outcomes[i]) {
            correct_predictions++;
        }
        if (predicted_class == 1 && (int)outcomes[i] == 1) {
            true_positive++;
        }
        if (predicted_class == 0 && (int)outcomes[i] == 0) {
            true_negative++;
        }
        if (predicted_class == 1 && (int)outcomes[i] == 0) {
            false_positive++;
        }
        if (predicted_class == 0 && (int)outcomes[i] == 1) {
            false_negative++;
        }
    }

    // Print confusion matrix
    printf("---------------------------------------------------\n");
    printf("True Positive: %d\t | False Positive: %d\n", true_positive, false_positive);
    printf("True Negative: %d\t | False Negative: %d\n", true_negative, false_negative);
    printf("----------------------------------------------------\n");

    return (double) correct_predictions / TRAINING_SIZE; // Return accuracy
}

// Parse the dataset from strings to integer arrays and outcomes
void parse_lr_dataset(char dataset[][COLUMNS][MAX_ARRAY_SIZE], int boards[][BOARD_SIZE], double outcomes[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            boards[i][j] = atoi(dataset[i][j]);  // Convert board values (strings) to integers
        }
        outcomes[i] = atof(dataset[i][9]);  // Convert outcome to double
    }
}

// Main function to create, train, and evaluate the model
int create_model(LinearRegressionModel *model, char training_set[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testing_set[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]) {
    initialize_model(model); // Initialize the model

    // Arrays to hold the parsed training and testing data
    int training_boards[TRAINING_SIZE][BOARD_SIZE];
    double training_outcomes[TRAINING_SIZE];

    int testing_boards[TESTING_SIZE][BOARD_SIZE];
    double testing_outcomes[TESTING_SIZE];

    // Parse the training and testing datasets
    parse_lr_dataset(training_set, training_boards, training_outcomes, TRAINING_SIZE);
    parse_lr_dataset(testing_set, testing_boards, testing_outcomes, TESTING_SIZE);

    // Train the model with the parsed training data
    train(model, training_boards, training_outcomes);

    // Evaluate the model's accuracy on the parsed testing data
    double testing_accuracy = evaluate_testing(model, testing_boards, testing_outcomes);
    printf("Model Accuracy: %.2f%%\n", testing_accuracy * 100);

    double training_accuracy = evaluate_training(model, training_boards, training_outcomes);
    printf("Model Accuracy: %.2f%%\n", training_accuracy * 100);

    return 0; // Return success
}