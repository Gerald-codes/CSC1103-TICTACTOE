To download the GTK dependency for Windows on MSYS2 MinGW64, follow these steps:

1. Install MSYS2 from the official website: https://www.msys2.org/
2. Open the MSYS2 MINGW64 terminal.
3. Update the package database and core system packages:
    ```sh
    pacman -Syu
    ```
4. Install the `pkg-config` package:
    ```sh
    pacman -S mingw-w64-x86_64-pkg-config
    ```
5. Install the GTK3 package:
    ```sh
    pacman -S mingw-w64-x86_64-gtk3
    ```

This will install the GTK3 library and its dependencies for the MinGW-w64 environment.

6. Change directory to our TIC-TAC-TOE game location

7. Run command "gcc -o start startingPage.c main.c mainMenu.c difficultyPage.c singlePlayer.c sharedFunction.c doublePlayer.c parseDataset.c minMax.c `pkg-config --cflags --libs gtk+-3.0`"

8. If above command results in error.
Run: gcc -o start startingPage.c main.c mainMenu.c difficultyPage.c singlePlayer.c sharedFunction.c doublePlayer.c parseDataset.c minMax.c $(pkg-config --cflags --libs gtk+-3.0)

9. Execute ./start file

Installation of Font

1. Under project font folder, open Jaro.ttf in file explorer
2. Click install on the top left