// Cristian Gramada, Justin Nacpil, Evan Nguyen, Ethan Wong

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

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
int dirX = 1, dirY = 0; // Initial direction: right
int gameOver = 0;

// Function declarations
void init();
void show_menu();
void draw();
void spawn_food();
void input();
void update();
void add_head(int x, int y);
void remove_tail();
int check_collision(int x, int y);
void end_game();

int main() {
    initscr(); // Initialize ncurses
    noecho(); // Don't echo user input
    curs_set(FALSE); // Hide the cursor
    keypad(stdscr, TRUE); 

    show_menu(); // Show menu and wait for space to start
    init();

    while (!gameOver) {
        input();
        update();
        draw();
        usleep(50000); // controls game speed (20 FPS)
    }

    end_game();
    return 0;
}

void show_menu() {
    erase();
    mvprintw(5, (WIDTH / 2) - 8, "Welcome to Terminal Snake!");
    mvprintw(7, (WIDTH / 2) - 10, "Controls:");
    mvprintw(8, (WIDTH / 2) - 10, "  - W : Move Up");
    mvprintw(9, (WIDTH / 2) - 10, "  - A : Move Left");
    mvprintw(10, (WIDTH / 2) - 10, "  - S : Move Down");
    mvprintw(11, (WIDTH / 2) - 10, "  - D : Move Right");
    mvprintw(12, (WIDTH / 2) - 10, "  - Q : Quit the game");
    mvprintw(14, (WIDTH / 2) - 10, "Press SPACE to start the game...");
    refresh();

    // Wait for spacebar to start the game
    timeout(-1); // Wait for input
    int ch;
    do {
        ch = getch();
    } while (ch != ' ');
    timeout(0); // Restore non-blocking mode
}

void init() {
    srand(time(0)); // random number generator based on current time
    nodelay(stdscr, TRUE); // changed from 50ms to no delay for less lag

    // Create initial snake head
    snake = malloc(sizeof(Node));
    snake->pos.x = WIDTH / 2;
    snake->pos.y = HEIGHT / 2;
    snake->next = NULL;

    score = 0;
    dirX = 1;
    dirY = 0;
    gameOver = 0;

    spawn_food();
}

void draw() {
    erase(); // lighter than clear() for less lag

    mvprintw(0, 0, "Score: %d", score); // Score header

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

    // Draw snake (head + body)
    Node *curr = snake;
    while (curr) {
        mvprintw(curr->pos.y + 2, curr->pos.x + 1, "O");
        curr = curr->next;
    }

    refresh(); // refreshes the screen to match memory
}

void input() {
    int ch = getch();
    switch (ch) {
        case 'w': case KEY_UP:
            if (dirY != 1) { dirX = 0; dirY = -1; } break;
        case 's': case KEY_DOWN:
            if (dirY != -1) { dirX = 0; dirY = 1; } break;
        case 'a': case KEY_LEFT:
            if (dirX != 1) { dirX = -1; dirY = 0; } break;
        case 'd': case KEY_RIGHT:
            if (dirX != -1) { dirX = 1; dirY = 0; } break;
        case 'q': // quit
            gameOver = 1; break;
    }
}

void update() {
    int newX = snake->pos.x + dirX;
    int newY = snake->pos.y + dirY;

    // Check for wall or self collision
    if (check_collision(newX, newY)) {
        gameOver = 1;
        return;
    }

    add_head(newX, newY);

    // Check for food collision
    if (newX == food.x && newY == food.y) {
        score += 10;
        spawn_food();
    } else {
        remove_tail();
    }
}

void add_head(int x, int y) {
    Node *newHead = malloc(sizeof(Node));
    newHead->pos.x = x;
    newHead->pos.y = y;
    newHead->next = snake;
    snake = newHead;
}

void remove_tail() {
    Node *curr = snake;
    if (!curr || !curr->next) return;

    while (curr->next->next) curr = curr->next;
    free(curr->next);
    curr->next = NULL;
}

int check_collision(int x, int y) {
    // Check for wall collision
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return 1;

    // Check for self collision
    Node *curr = snake;
    while (curr) {
        if (curr->pos.x == x && curr->pos.y == y) return 1;
        curr = curr->next;
    }
    return 0;
}

void spawn_food() {
    int valid = 0;
    while (!valid) {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        valid = 1;
        Node *curr = snake;
        while (curr) {
            if (curr->pos.x == food.x && curr->pos.y == food.y) {
                valid = 0;
                break;
            }
            curr = curr->next;
        }
    }
}

void end_game() {
    erase();
    mvprintw(HEIGHT / 2, WIDTH / 2 - 6, "Game Over!");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 8, "Final Score: %d", score);
    mvprintw(HEIGHT / 2 + 2, WIDTH / 2 - 14, "Press 'q' or 'Q' to exit...");
    refresh();
    timeout(-1);  // wait for input

    // Wait for q or Q key to exit
    int ch;
    do {
        ch = getch();
    } while (ch != 'q' && ch != 'Q');

    endwin();

    // Free memory
    while (snake) {
        Node *temp = snake;
        snake = snake->next;
        free(temp);
    }
}
