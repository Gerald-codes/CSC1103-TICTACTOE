
#include <gtk/gtk.h>

#ifndef PAGE_H   // Check if HEADER_NAME_H is not defined
#define PAGE_H   // Define HEADER_NAME_H

#define BOARD_SIZE 9  // 3x3 Tic-Tac-Toe board

typedef struct {
    double weights[BOARD_SIZE];  // Weights for each cell on the board
    double bias;                 // Bias term

} LinearRegressionModel;

extern LinearRegressionModel model;

void show_main_menu_page(GtkWidget *starting_window);
void show_difficulty_page(GtkWidget *main_menu_window);
void show_single_player_page(GtkWidget *difficulty_window);
void show_double_player_page(GtkWidget *main_menu_window);
int parseDataset(LinearRegressionModel *model);



#endif // HEADER_NAME_H  // End of header guard