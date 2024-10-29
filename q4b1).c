#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_SIZE 1024
#define ROWS 958
#define COLUMNS 10
#define MAX_ARRAY_SIZE 100
void readFile (const char *filename,char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]);
void splitDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char trainingSet[(int)(ROWS * 0.8)][COLUMNS][MAX_ARRAY_SIZE], char testingSet[(int)(ROWS * 0.2)][COLUMNS][MAX_ARRAY_SIZE]);

// Function to read the file
void readFile(const char *filename,char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE]){
    // file pointer
    FILE *dataset;
    //open the file in read mode
    dataset = fopen(filename,"r");
    if(dataset == NULL){
        printf("Error opening file\n");
        exit(1);
    }

    char line[LINE_BUFFER_SIZE];
    int row;
    while (fgets(line, sizeof(line), dataset) &&row<ROWS )
    {
        //remove the newline character
        line[strcspn(line, "\n")] = 0;
        int col = 0;

        //tokenize the line
        char *token = strtok(line, ",");
        while (token != NULL && col<COLUMNS) {
            strcpy(tictactoeDataset[row][col], token);
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
    fclose(dataset);
}


void splitDataset(char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE], char trainingSet[(int)(ROWS * 0.8)][COLUMNS][MAX_ARRAY_SIZE], char testingSet[(int)(ROWS * 0.2)][COLUMNS][MAX_ARRAY_SIZE]){
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


int main(){
    char tictactoeDataset[ROWS][COLUMNS][MAX_ARRAY_SIZE];
    const char *filename = "tic-tac-toe.data";
    readFile(filename,tictactoeDataset);
    // Split tictactoeDataset into 2 sets, 80% for training, 20% for testing
    char trainingSet[(int)(ROWS * 0.8)][COLUMNS][MAX_ARRAY_SIZE];
    char testingSet[(int)(ROWS * 0.2)][COLUMNS][MAX_ARRAY_SIZE];
// Now you can use trainingSet and testingSet for further processing
    splitDataset(tictactoeDataset, trainingSet, testingSet);
//print the whole training and testing set with row number
    for (int i = 0; i < (int)(ROWS * 0.8); i++) {
        printf("Training Row %d: ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%s ", trainingSet[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < (int)(ROWS * 0.2); i++) {
        printf("Testing Row %d: ", i);
        for (int j = 0; j < COLUMNS; j++) {
            printf("%s ", testingSet[i][j]);
        }
        printf("\n");
    }
        return 0;
}
