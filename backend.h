#define BOARD_SIZE 9  // 3x3 Tic-Tac-Toe board

typedef struct {
    double weights[BOARD_SIZE];  // Weights for each cell on the board
    double bias;                 // Bias term

} LinearRegressionModel;

extern LinearRegressionModel model;

int parseDataset(LinearRegressionModel *model);
int findBestMove(LinearRegressionModel *model);
int findRandomMinMaxMove();
int findBestMinMaxMove();
