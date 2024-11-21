#define BOARD_SIZE 9  // 3x3 Tic-Tac-Toe board

typedef struct {
    double weights[BOARD_SIZE];  // Weights for each cell on the board
    double bias;                 // Bias term

} LinearRegressionModel;

extern LinearRegressionModel model;

int parse_dataset(LinearRegressionModel *model);
int find_best_move(LinearRegressionModel *model);
int find_random_min_max_move();
int find_best_min_max_move();
