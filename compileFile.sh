gcc -o start main.c main_menu.c difficulty_page.c shared_function.c single_player.c double_player.c parseDataset.c minMax.c startingPage.c `pkg-config --cflags --libs gtk+-3.0`
./start
