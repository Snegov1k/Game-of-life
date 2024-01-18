#include <curses.h>
#include <stdio.h>
#include <unistd.h>

#define ROWS 25
#define COLUMNS 80

void input(int binary_field[ROWS][COLUMNS], int *error);
void menu();
void make_field(char field[ROWS][COLUMNS], int binary_field[ROWS][COLUMNS]);
void update_the_field(int field[ROWS][COLUMNS]);
int update_points(int old_field[ROWS][COLUMNS], int y, int x);
void copy_array(int array_first[ROWS][COLUMNS], int array_second[ROWS][COLUMNS]);
int check_for_borders(int var, int border);

int main() {
    int binary_field[ROWS][COLUMNS] = {0};
    int error = 0;

    input(binary_field, &error);
    FILE *p = freopen("/dev/tty", "r", stdin);

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    if (error == 0) {
        menu();

        getchar();

        char field[ROWS][COLUMNS];
        int speed = 100000;

        while (1) {
            char choice = getch();

            if (choice == 'q' || choice == 'Q') break;
            if (choice == 'z' || choice == 'Z') speed += 25000;
            if (choice == 'a' || choice == 'A') speed = (speed - 25000 < 0) ? 0 : speed - 25000;

            update_the_field(binary_field);

            make_field(field, binary_field);

            usleep(speed);
        }

    } else {
        printf("n/a");
    }

    endwin();

    fclose(p);

    return 0;
}

void input(int binary_field[ROWS][COLUMNS], int *error) {
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLUMNS - 1; j++) {
            if (!scanf("%d", &binary_field[i][j])) {
                *error = 1;
            }
        }
    }
}

void menu() {
    clear();

    printw(
        "It is the realization of the famous game \"Life\" which is a cellular automaton invented by the "
        "English mathematician John Conway. We hope you'll enjoy it\n");
    printw("Game rules:\n");
    printw("\tAny live cell with fewer than two live neighbours dies, as if by underpopulation.\n");
    printw("\tAny live cell with two or three live neighbours lives on to the next generation.\n");
    printw("\tAny live cell with more than three live neighbours dies, as if by overpopulation.\n");
    printw(
        "\tAny dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.\n");
    printw("Control buttons\n");
    printw("\t\"q\" - exit\n");
    printw("\t\"a\" - speed up\n");
    printw("\t\"z\" - slow down\n");
    printw("Press any button to continue\n");

    refresh();
}

void make_field(char field[ROWS][COLUMNS], int binary_field[ROWS][COLUMNS]) {
    clear();

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            if (j == 0 || j == 79 || i == 0 || i == 24) {
                field[i][j] = '#';
            } else if (binary_field[i][j] == 1) {
                field[i][j] = 'o';
            } else {
                field[i][j] = ' ';
            }
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            printw("%c", field[i][j]);
        }
        printw("\n");
    }

    refresh();
}

void update_the_field(int binary_field[ROWS][COLUMNS]) {
    int old_binary_field[ROWS][COLUMNS];

    copy_array(old_binary_field, binary_field);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            binary_field[i][j] = update_points(old_binary_field, i, j);
        }
    }
}

int update_points(int old_binary_field[ROWS][COLUMNS], int y, int x) {
    int result = 0, count = 0;

    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;

            if (old_binary_field[check_for_borders(i + y, ROWS)][check_for_borders(j + x, COLUMNS)]) count++;
        }
    }

    if ((old_binary_field[y][x] == 0 && count == 3) ||
        (old_binary_field[y][x] == 1 && (count == 2 || count == 3)))
        result = 1;

    return result;
}

void copy_array(int array_first[ROWS][COLUMNS], int array_second[ROWS][COLUMNS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            array_first[i][j] = array_second[i][j];
        }
    }
}

int check_for_borders(int var, int border) {
    if (var == 0) var = border - 2;
    if (var == border - 1) var = 1;

    return var;
}
