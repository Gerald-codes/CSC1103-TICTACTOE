gcc -o start startingPage.c main_menu.c difficulty_page.c shared_function.c single_player.c double_player.c `pkg-config --cflags --libs gtk+-3.0`
./start
