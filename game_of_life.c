#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 80
#define HEIGHT 25

void read_file(const char* filename, char grid[HEIGHT][WIDTH]);
void printt(char grid[HEIGHT][WIDTH]);
int count_n(char grid[HEIGHT][WIDTH], int x, int y);
void update_grid(char grid[HEIGHT][WIDTH]);
int show_zastavka();
char* get_filename(int pattern_number);
void game_loop(char grid[HEIGHT][WIDTH]);

int main() {
    char grid[HEIGHT][WIDTH];

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, FALSE);

    int pattern_number = show_zastavka();
    nodelay(stdscr, TRUE);

    char* filename = get_filename(pattern_number);
    read_file(filename, grid);

    game_loop(grid);
    endwin();

    return 0;
}

char* get_filename(int pattern_number) {
    static char filename[50];
    if (pattern_number == 1)
        sprintf(filename, "pattern/acorn.txt");
    else if (pattern_number == 2)
        sprintf(filename, "pattern/B-Heptomino.txt");
    else if (pattern_number == 3)
        sprintf(filename, "pattern/Blinker_Fuse.txt");
    else if (pattern_number == 4)
        sprintf(filename, "pattern/Glider_by_the_dozen.txt");
    else if (pattern_number == 5)
        sprintf(filename, "pattern/Pentadecathlon.txt");
    else if (pattern_number == 6)
        sprintf(filename, "pattern/Pentadecathlon2.txt");
    else if (pattern_number == 7)
        sprintf(filename, "pattern/Piheptomino.txt");
    else if (pattern_number == 8)
        sprintf(filename, "pattern/Wheel.txt");
    else if (pattern_number == 9)
        sprintf(filename, "pattern/our_pattern.txt");
    else if (pattern_number == 10)
        sprintf(filename, "pattern/oscillator_period_177.txt");
    else if (pattern_number == 11)
        sprintf(filename, "pattern/Happy_chellic_by_Ivan.txt");
    else
        sprintf(filename, "pattern/acorn.txt");

    return filename;
}

void game_loop(char grid[HEIGHT][WIDTH]) {
    int generation = 0;
    int game = 1;
    int k = 500;

    while (game) {
        printt(grid);
        mvprintw(HEIGHT, 0, "Generation: %d - Press SPACE BAR to quit", generation++);
        mvprintw(HEIGHT + 1, 0, "Speed: %d", k);
        refresh();

        update_grid(grid);

        int ch = getch();
        if (ch == ' ')
            game = 0;
        else if ((ch == 'a' || ch == 'A') && k >= 300) {
            k -= 200;
        } else if ((ch == 'z' || ch == 'Z') && k <= 1300) {
            k += 200;
        }

        napms(k);
    }
}

int show_zastavka() {
    clear();
    int k = 1;
    char input[3] = "";
    attron(A_BOLD);
    mvprintw(5, 20, "*** Game of life ***");
    attroff(A_BOLD);

    mvprintw(7, 10, "Rules:");
    mvprintw(8, 12, "- Live cell with 2-3 neighbors survives");
    mvprintw(9, 12, "- Dead cell with 3 neighbors becomes alive");
    mvprintw(10, 12, "- Other cells die or stay dead");

    mvprintw(12, 10, "Controls:");
    mvprintw(13, 12, "A - increase speed");
    mvprintw(14, 12, "Z - decrease speed");
    mvprintw(15, 12, "SPACE - exit");

    mvprintw(17, 10, "Patterns:");
    mvprintw(18, 12, "1. Acorn         2. B-Heptomino    3. Blinker Fuse");
    mvprintw(19, 12, "4. Glider dozen  5. Pentadecathlon 6. Pentadecathlon2");
    mvprintw(20, 12, "7. Piheptomino   8. Wheel          9. Our pattern");
    mvprintw(21, 12, "10. Oscillator   ");

    mvprintw(23, 12, "Enter pattern number (1-10): ");
    refresh();
    echo();
    curs_set(1);
    getnstr(input, 2);
    noecho();
    curs_set(0);
    k = atoi(input);
    if (k < 1)
        k = 1;
    else if (k > 12)
        k = 12;
    clear();
    refresh();

    return k;
}

int count_n(char grid[HEIGHT][WIDTH], int x, int y) {
    int k = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(j == 0 && i == 0)) {
                int nx = (x + j + WIDTH) % WIDTH;
                int ny = (y + i + HEIGHT) % HEIGHT;
                if (grid[ny][nx] == '#') k++;
            }
        }
    }
    return k;
}

void update_grid(char grid[HEIGHT][WIDTH]) {
    char new_grid[HEIGHT][WIDTH];

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int n = count_n(grid, j, i);
            if (grid[i][j] == '#') {
                if (n == 2 || n == 3)
                    new_grid[i][j] = '#';
                else
                    new_grid[i][j] = '.';
            } else {
                if (n == 3)
                    new_grid[i][j] = '#';
                else
                    new_grid[i][j] = '.';
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++) grid[i][j] = new_grid[i][j];
}

void printt(char grid[HEIGHT][WIDTH]) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) printw("%c", grid[i][j]);
        printw("\n");
    }
}

void read_file(const char* filename, char grid[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grid[i][j] = '.';
        }
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }

    int i = 0;
    char line[WIDTH + 2];

    while (i < HEIGHT && fgets(line, sizeof(line), file) != NULL) {
        for (int j = 0; j < WIDTH && line[j] != '\0' && line[j] != '\n'; j++) {
            if (line[j] == '#') {
                grid[i][j] = '#';
            }
        }
        i++;
    }

    fclose(file);
}