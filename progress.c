#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 30
#define HEIGHT 15

typedef struct {
    int x, y;
} Point;

typedef struct Node {
    Point pos;
    struct Node *next;
} Node;

Node *snake = NULL;
Point food;
int score = 0;

void init();
void draw();
void spawn_food();

int main() {
    init();

    for (int i = 0; i < 10; i++) {
        draw();
        usleep(300000); // delay to slow down snake
    }

    endwin(); // deallocates memory and ends ncurses
    return 0;
}

void init() {
    srand(time(0)); // for rand() to spawn food at random locations every time
    initscr(); // initializes the window
    noecho(); // prevents user input from being displayed on the screen
    curs_set(FALSE); // hide the cursor
    keypad(stdscr, TRUE); // enable arrow keys
    timeout(100); // waits for user input for 100 milliseconds

    // Create initial snake head
    snake = malloc(sizeof(Node));
    snake->pos.x = WIDTH / 2;
    snake->pos.y = HEIGHT / 2;
    snake->next = NULL;

    score = 0;
    spawn_food();
}

void draw() {
    clear();

    // Score header
    mvprintw(0, 0, "Score: %d", score);

    // Draw borders
    for (int x = 0; x < WIDTH + 2; x++) mvprintw(1, x, "#");
    for (int y = 0; y < HEIGHT; y++) {
        mvprintw(y + 2, 0, "#");
        for (int x = 0; x < WIDTH; x++) mvprintw(y + 2, x + 1, " ");
        mvprintw(y + 2, WIDTH + 1, "#");
    }
    for (int x = 0; x < WIDTH + 2; x++) mvprintw(HEIGHT + 2, x, "#");

    // Draw food
    mvprintw(food.y + 2, food.x + 1, "@");

    // Draw snake
    mvprintw(snake->pos.y + 2, snake->pos.x + 1, "O");

    refresh(); //refreshes the screen to match what is in memory
}

void spawn_food() {
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
}