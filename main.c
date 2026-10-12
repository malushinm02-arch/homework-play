
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>

char *board;
int size;

void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void showBoard() {
    printf("Field: %d x %d\n\n", size, size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int index = i * size + j;

            if (board[index] == '.') {
                printf("%3d", index + 1);
            } else {
                printf("%3c", board[index]);
            }

            if (j < size - 1) {
                printf(" |");
            }
        }

        printf("\n");

        if (i < size - 1) {
            for (int j = 0; j < size; j++) {
                printf("----");
                if (j < size - 1) {
                    printf("+");
                }
            }
            printf("\n");
        }
    }

    printf("\n");
}

int checkWin(char symbol) {
    for (int i = 0; i < size; i++) {
        int row = 1;
        int col = 1;

        for (int j = 0; j < size; j++) {
            if (board[(size_t)i * size + j] != symbol) {
                row = 0;
            }

            if (board[(size_t)j * size + i] != symbol) {
                col = 0;
            }
        }

        if (row || col) {
            return 1;
        }
    }

    int diagonal1 = 1;
    int diagonal2 = 1;

    for (int i = 0; i < size; i++) {
        if (board[(size_t)i * size + i] != symbol) {
            diagonal1 = 0;
        }

        if (board[(size_t)i * size + (size - 1 - i)] != symbol) {
            diagonal2 = 0;
        }
    }

    return diagonal1 || diagonal2;
}

int main() {
    int mode;
    int choice;
    long long move;
    char player;
    char bot;
    char current;

    srand((unsigned int)time(NULL));

    printf("Choose field size (minimum 3): ");

    if (scanf("%d", &size) != 1 || size < 3) {
        printf("Invalid field size!\n");
        return 1;
    }

    if ((size_t)size > SIZE_MAX / (size_t)size) {
        printf("Field is too large!\n");
        return 1;
    }

    size_t cells = (size_t)size * (size_t)size;

    if (cells > SIZE_MAX / sizeof(char) ||
        cells > (size_t)LLONG_MAX) {
        printf("Field is too large!\n");
        return 1;
    }

    board = malloc(cells * sizeof(char));

    if (board == NULL) {
        printf("Not enough memory!\n");
        return 1;
    }

    printf("\n1 - Play with bot\n");
    printf("2 - Play with another player\n");
    printf("Choose: ");

    if (scanf("%d", &mode) != 1 ||
        (mode != 1 && mode != 2)) {
        free(board);
        return 1;
    }

    printf("\n1 - X\n");
    printf("2 - O\n");
    printf("Choose your symbol: ");

    if (scanf("%d", &choice) != 1 ||
        (choice != 1 && choice != 2)) {
        free(board);
        return 1;
    }

    player = choice == 1 ? 'X' : 'O';
    bot = player == 'X' ? 'O' : 'X';

    for (size_t i = 0; i < cells; i++) {
        board[i] = '.';
    }

    current = 'X';

    while (1) {
        clearScreen();
        showBoard();

        if (mode == 1 && current == bot) {
            size_t position;

            do {
                position = (size_t)rand() % cells;
            } while (board[position] != '.');

            board[position] = bot;
        } else {
            printf("Player %c, choose cell (1-%zu): ",
                   current, cells);
            fflush(stdout);

            if (scanf("%lld", &move) != 1) {
                free(board);
                return 1;
            }

            if (move < 1 ||
                (size_t)move > cells ||
                board[(size_t)(move - 1)] != '.') {
                printf("Invalid move! Press Enter to continue.");
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {
                }
                getchar();
                continue;
            }

            board[(size_t)(move - 1)] = current;
        }

        if (checkWin(current)) {
            clearScreen();
            showBoard();
            printf("Player %c wins!\n", current);
            break;
        }

        int full = 1;

        for (size_t i = 0; i < cells; i++) {
            if (board[i] == '.') {
                full = 0;
                break;
            }
        }

        if (full) {
            clearScreen();
            showBoard();
            printf("Draw!\n");
            break;
        }

        current = current == 'X' ? 'O' : 'X';
    }

    free(board);
    return 0;
}
