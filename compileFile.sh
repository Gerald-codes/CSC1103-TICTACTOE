gcc -o start startingPage.c main_menu.c difficulty_page.c tic_tac_toe.c parseDataset.c minMax.c `pkg-config --cflags --libs gtk+-3.0`
./start
