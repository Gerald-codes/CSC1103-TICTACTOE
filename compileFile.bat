gcc -o start startingPage.c main.c mainMenu.c difficultyPage.c singlePlayer.c sharedFunction.c doublePlayer.c parseDataset.c minMax.c `pkg-config --cflags --libs gtk+-3.0`
./start
