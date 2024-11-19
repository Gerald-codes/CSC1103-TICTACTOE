cd /Users/gerald/Desktop/CSC1103-TICTACTOE/
gcc -o start startingPage.c main_menu.c difficulty_page.c tic_tac_toe.c parseDataset.c `pkg-config --cflags --libs gtk+-3.0`
./start
