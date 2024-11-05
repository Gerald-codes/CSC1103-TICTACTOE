#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_SIZE 9  // 3x3 Tic-Tac-Toe board
#define LEARNING_RATE 0.0001
#define EPOCHS 1000
#define MAX_SAMPLES 958
#define TRAINING_SIZE (int)(958 * 0.8)
#define TESTING_SIZE (int)(958 * 0.2)
#define COLUMNS 10
#define MAX_ARRAY_SIZE 100


// Linear regression model
typedef struct {
    double weights[BOARD_SIZE];  // Weights for each cell on the board
    double bias;                 // Bias term

} LinearRegressionModel;


// Function Declarations
void initialize_model(LinearRegressionModel *model);
double predict(LinearRegressionModel *model, int board[]);
void train(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]);
double evaluate(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]);
int createModel(LinearRegressionModel *model,char trainingset[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testingset[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]);
void parse_dataset(char dataset[][COLUMNS][MAX_ARRAY_SIZE], int boards[][BOARD_SIZE], double outcomes[], int size);


// Function to initialize the model with random weights
void initialize_model(LinearRegressionModel *model) {
    // for (int i = 0; i < BOARD_SIZE; i++) {
    //     model->weights[i] = (double) rand() / RAND_MAX;  // Random weight initialization
    // }
    double initial_weights[BOARD_SIZE] = {0.8, 0.4, 0.8,
                                          0.4, 1.0, 0.4,
                                          0.8, 0.4, 0.8};
    for (int i = 0; i < BOARD_SIZE; i++) {
        model->weights[i] = initial_weights[i];
    }
    model->bias = (double) rand() / RAND_MAX;  // Random bias initialization
}

// Predict the outcome for a given board state
double predict(LinearRegressionModel *model, int board[]) {
    double score = model->bias;
    for (int i = 0; i < BOARD_SIZE; i++) {
        score += model->weights[i] * board[i];
    }
    return score;
}

// Train the model using gradient descent
void train(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]) {
    for (int epoch = 0; epoch < EPOCHS; epoch++) {
        double total_error = 0;
        for (int i = 0; i < TRAINING_SIZE; i++) {

            // Get the prediction
            double prediction = predict(model, boards[i]);
            double error = prediction - outcomes[i];  // Calculate error
            // Update weights and bias using gradient descent
            for (int j = 0; j < BOARD_SIZE; j++) {
                model->weights[j] -= LEARNING_RATE * error * boards[i][j];  // Update weight
            }
            model->bias -= LEARNING_RATE * error;  // Update bias

            total_error += error * error;  // Mean squared error
        }

        // Optionally print the loss every 100 epochs
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

// Function to evaluate the model's accuracy
double evaluate(LinearRegressionModel *model, int boards[][BOARD_SIZE], double outcomes[]) {
    int correct_predictions = 0;
    for (int i = 0; i < TESTING_SIZE; i++) {
        double prediction = predict(model, boards[i]);
        int predicted_class = (prediction >= 0.5) ? 1 : 0;  // Threshold at 0.5

        if (predicted_class == (int)outcomes[i]) {
            correct_predictions++;
        }
    }
    return (double) correct_predictions / TESTING_SIZE;
}

// Parse the dataset from strings to integer arrays and outcomes
void parse_dataset(char dataset[][COLUMNS][MAX_ARRAY_SIZE], int boards[][BOARD_SIZE], double outcomes[], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            boards[i][j] = atoi(dataset[i][j]);  // Convert board values (strings) to integers
        }
        printf("OUTCOME %d) \t %d \n", i,atoi(dataset[i][-1]));
        outcomes[i] = atof(dataset[i][9]);  // Convert outcome to double
    }
}

int createModel(LinearRegressionModel *model,char trainingset[TRAINING_SIZE][COLUMNS][MAX_ARRAY_SIZE], char testingset[TESTING_SIZE][COLUMNS][MAX_ARRAY_SIZE]) {
    initialize_model(model);

    // Arrays to hold the parsed training and testing data
    int training_boards[TRAINING_SIZE][BOARD_SIZE];
    double training_outcomes[TRAINING_SIZE];

    int testing_boards[TESTING_SIZE][BOARD_SIZE];
    double testing_outcomes[TESTING_SIZE];

    // Parse the training and testing datasets
    parse_dataset(trainingset, training_boards, training_outcomes, TRAINING_SIZE);
    parse_dataset(testingset, testing_boards, testing_outcomes, TESTING_SIZE);

    // Train the model with the parsed training data
    train(model, training_boards, training_outcomes);

    // Evaluate the model's accuracy on the parsed testing data
    double accuracy = evaluate(model, testing_boards, testing_outcomes);
    printf("Model Accuracy: %.2f%%\n", accuracy * 100);

    return 0;
}


