To download the GTK dependency for MacOS, follow these steps:

1. Install brew:
2. ```sh
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```
3. Install GTK3 package and font:
   run the setup.sh script
   ```sh
    ./setup.sh
   ```
           OR
   ```sh
   brew install gtk+3
   brew install --cask font-jaro
   ```

This will install the GTK3 library and its dependencies for the MacOS environment.

To start our TIC-TAC-TOE game, follow these steps:
1. Change directory to our TIC-TAC-TOE game location

2. Compile file using GCC
  ```sh
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
  ```
3. Start the game
   ```sh
   ./start
   ```

ANOTHER WAY
1. Execute the compileFile script
   ```sh
   ./compileFile.sh
   ```
