gcc -o start \
    main.c \
    startingPage.c \
    mainMenu.c \
    difficultyPage.c \
    sharedFunction.c \
    singlePlayer.c \
    doublePlayer.c \
    parseDataset.c \
    minMax.c \
    song.m \
    `pkg-config --cflags --libs gtk+-3.0` \
    -framework AudioToolbox \
    -framework AVFoundation \
    -framework Foundation

./start
