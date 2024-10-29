#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 300
#define SCREEN_HEIGHT 300
#define CELL_SIZE 100

char board[9] = {'-', '-', '-', '-', '-', '-', '-', '-', '-'};
char currentPlayer = 'X';

void printBoard() {
    for (int i = 0; i < 9; i++) {
        printf("%c ", board[i]);
        if ((i + 1) % 3 == 0) printf("\n");
    }
    printf("\n");
}

char* checkGameOver() {
    if ((board[0] == board[1] && board[1] == board[2] && board[0] != '-') ||
        (board[3] == board[4] && board[4] == board[5] && board[3] != '-') ||
        (board[6] == board[7] && board[7] == board[8] && board[6] != '-') ||
        (board[0] == board[3] && board[3] == board[6] && board[0] != '-') ||
        (board[1] == board[4] && board[4] == board[7] && board[1] != '-') ||
        (board[2] == board[5] && board[5] == board[8] && board[2] != '-') ||
        (board[0] == board[4] && board[4] == board[8] && board[0] != '-') ||
        (board[2] == board[4] && board[4] == board[6] && board[2] != '-')) {
        return "win";
    }
    
    int isTie = 1;
    for (int i = 0; i < 9; i++) {
        if (board[i] == '-') {
            isTie = 0;
            break;
        }
    }
    return isTie ? "tie" : "play";
}

void renderBoard(SDL_Renderer *renderer) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Draw grid lines
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 1; i <= 2; i++) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }

    // Draw X and O
    for (int i = 0; i < 9; i++) {
        int row = i / 3;
        int col = i % 3;
        int x = col * CELL_SIZE;
        int y = row * CELL_SIZE;

        if (board[i] == 'X') {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawLine(renderer, x + 10, y + 10, x + CELL_SIZE - 10, y + CELL_SIZE - 10);
            SDL_RenderDrawLine(renderer, x + CELL_SIZE - 10, y + 10, x + 10, y + CELL_SIZE - 10);
        } else if (board[i] == 'O') {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_Rect rect = { x + 10, y + 10, CELL_SIZE - 20, CELL_SIZE - 20 };
            SDL_RenderDrawRect(renderer, &rect);
        }
    }

    // Update screen
    SDL_RenderPresent(renderer);
}

void takeTurn(int position) {
    if (position >= 0 && position < 9 && board[position] == '-') {
        board[position] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int quit = 0;
    SDL_Event e;

    printBoard();
    renderBoard(renderer);

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int col = x / CELL_SIZE;
                int row = y / CELL_SIZE;
                int position = row * 3 + col;

                takeTurn(position);
                renderBoard(renderer);
                printBoard();

                char* gameResult = checkGameOver();
                if (strcmp(gameResult, "win") == 0) {
                    printf("%c wins!\n", (currentPlayer == 'X') ? 'O' : 'X');
                    quit = 1;
                } else if (strcmp(gameResult, "tie") == 0) {
                    printf("It's a tie!\n");
                    quit = 1;
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
