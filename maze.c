#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define MAZE_WIDTH 20
#define MAZE_HEIGHT 15
#define CELL_SIZE 30

struct Cell {
    int visited;
    int walls[4]; // top, right, bottom, left
};

struct Cell maze[MAZE_HEIGHT][MAZE_WIDTH];

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

void initializeMaze()
{
    int y, x, i;
    for(y = 0; y < MAZE_HEIGHT; y++) {
        for(x = 0; x < MAZE_WIDTH; x++) {
            maze[y][x].visited = 0;
            for(i = 0; i < 4; i++)
                maze[y][x].walls[i] = 1;
        }
    }
}

void drawCell(int x, int y)
{
    int px = x * CELL_SIZE;
    int py = y * CELL_SIZE;

    setcolor(BLACK);
    if(maze[y][x].walls[0]) line(px, py, px + CELL_SIZE, py);                   // top
    if(maze[y][x].walls[1]) line(px + CELL_SIZE, py, px + CELL_SIZE, py + CELL_SIZE); // right
    if(maze[y][x].walls[2]) line(px, py + CELL_SIZE, px + CELL_SIZE, py + CELL_SIZE); // bottom
    if(maze[y][x].walls[3]) line(px, py, px, py + CELL_SIZE);                   // left
}

void drawMaze()
{
    int y, x;
    cleardevice();
    for(y = 0; y < MAZE_HEIGHT; y++)
        for(x = 0; x < MAZE_WIDTH; x++)
            drawCell(x, y);
}

int valid(int nx, int ny)
{
    return (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT && maze[ny][nx].visited == 0);
}

void removeWall(int x1, int y1, int x2, int y2)
{
    if(x1 == x2) {
        if(y1 > y2) {
            maze[y1][x1].walls[0] = 0;
            maze[y2][x2].walls[2] = 0;
        } else {
            maze[y1][x1].walls[2] = 0;
            maze[y2][x2].walls[0] = 0;
        }
    } else if(y1 == y2) {
        if(x1 > x2) {
            maze[y1][x1].walls[3] = 0;
            maze[y2][x2].walls[1] = 0;
        } else {
            maze[y1][x1].walls[1] = 0;
            maze[y2][x2].walls[3] = 0;
        }
    }
}

void generateMaze(int startX, int startY)
{
    int pathX[MAZE_WIDTH * MAZE_HEIGHT];
    int pathY[MAZE_WIDTH * MAZE_HEIGHT];
    int top = 0;
    int i, x, y, nx, ny, count, dir;
    int neighbors[4];

    maze[startY][startX].visited = 1;
    pathX[top] = startX;
    pathY[top] = startY;

    while(top >= 0)
    {
        x = pathX[top];
        y = pathY[top];
        count = 0;

        for(i = 0; i < 4; i++) {
            nx = x + dx[i];
            ny = y + dy[i];
            if(valid(nx, ny)) {
                neighbors[count] = i;
                count++;
            }
        }

        if(count > 0) {
            dir = neighbors[rand() % count];
            nx = x + dx[dir];
            ny = y + dy[dir];

            removeWall(x, y, nx, ny);
            maze[ny][nx].visited = 1;

            top++;
            pathX[top] = nx;
            pathY[top] = ny;
        } else {
            top--;
        }
    }

    // Reset visited for solving
    for(y = 0; y < MAZE_HEIGHT; y++)
        for(x = 0; x < MAZE_WIDTH; x++)
            maze[y][x].visited = 0;
}

void fillCell(int x, int y, int color)
{
    int px = x * CELL_SIZE + 1;
    int py = y * CELL_SIZE + 1;
    int size = CELL_SIZE - 1;

    setfillstyle(SOLID_FILL, color);
    bar(px, py, px + size, py + size);
}

int solveMaze(int x, int y, int endX, int endY)
{
    int i, nx, ny;

    if(x == endX && y == endY) {
        fillCell(x, y, GREEN);
        return 1;
    }

    maze[y][x].visited = 1;
    fillCell(x, y, YELLOW);
    delay(30);

    for(i = 0; i < 4; i++) {
        nx = x + dx[i];
        ny = y + dy[i];

        if(nx < 0 || nx >= MAZE_WIDTH || ny < 0 || ny >= MAZE_HEIGHT)
            continue;
        if(maze[ny][nx].visited)
            continue;
        if(i == 0 && maze[y][x].walls[0])
            continue;
        if(i == 1 && maze[y][x].walls[1])
            continue;
        if(i == 2 && maze[y][x].walls[2])
            continue;
        if(i == 3 && maze[y][x].walls[3])
            continue;

        if(solveMaze(nx, ny, endX, endY)) {
            fillCell(x, y, GREEN);
            return 1;
        }
    }

    fillCell(x, y, RED);
    delay(20);
    return 0;
}

int main()
{
    int win_width = MAZE_WIDTH * CELL_SIZE;
    int win_height = MAZE_HEIGHT * CELL_SIZE;

    srand(time(NULL));

    initwindow(win_width, win_height, "Maze Generator and Solver");
    setbkcolor(WHITE);
    cleardevice();

    initializeMaze();
    generateMaze(0, 0);
    drawMaze();
    solveMaze(0, 0, MAZE_WIDTH - 1, MAZE_HEIGHT - 1);

    delay(2000);
    closegraph(CURRENT_WINDOW);

    return 0;
}
