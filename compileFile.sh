gcc -o start main.c startingPage.c mainMenu.c difficultyPage.c sharedFunction.c singlePlayer.c doublePlayer.c parseDataset.c minMax.c `pkg-config --cflags --libs gtk+-3.0`
./start
