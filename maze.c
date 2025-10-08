#include <graphics.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int maze_width = 20;
const int maze_height = 15;
const int cell_size = 30;

struct Cell {
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // top, right, bottom, left
};

Cell maze[maze_height][maze_width];

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

void drawCell(int x, int y)
{
    int px = x * cell_size;
    int py = y * cell_size;

    setcolor(BLACK);
    if(maze[y][x].walls[0]) line(px, py, px + cell_size, py);       // top
    if(maze[y][x].walls[1]) line(px + cell_size, py, px + cell_size, py + cell_size); // right
    if(maze[y][x].walls[2]) line(px, py + cell_size, px + cell_size, py + cell_size); // bottom
    if(maze[y][x].walls[3]) line(px, py, px, py + cell_size);       // left
}

void drawMaze()
{
    cleardevice();
    for(int y=0; y<maze_height; y++)
        for(int x=0; x<maze_width; x++)
            drawCell(x, y);
}

bool valid(int nx, int ny) {
    return (nx >= 0 && nx < maze_width && ny >= 0 && ny < maze_height && !maze[ny][nx].visited);
}

void removeWall(int x1, int y1, int x2, int y2)
{
    if(x1 == x2) {
        if(y1 > y2) { maze[y1][x1].walls[0] = false; maze[y2][x2].walls[2] = false; }
        else { maze[y1][x1].walls[2] = false; maze[y2][x2].walls[0] = false; }
    } else if(y1 == y2) {
        if(x1 > x2) { maze[y1][x1].walls[3] = false; maze[y2][x2].walls[1] = false; }
        else { maze[y1][x1].walls[1] = false; maze[y2][x2].walls[3] = false; }
    }
}

// DFS
void generateMaze(int startX, int startY)
{
    int pathX[maze_width * maze_height];
    int pathY[maze_width * maze_height];
    int top = 0;

    maze[startY][startX].visited = true;
    pathX[top] = startX;
    pathY[top] = startY;

    while(top >= 0)
    {
        int x = pathX[top];
        int y = pathY[top];

        int neighbors[4];
        int count = 0;
        for(int i=0; i<4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if(valid(nx, ny)) neighbors[count++] = i;
        }

        if(count > 0) {
            int dir = neighbors[rand() % count];
            int nx = x + dx[dir];
            int ny = y + dy[dir];

            removeWall(x, y, nx, ny);
            maze[ny][nx].visited = true;

            top++;
            pathX[top] = nx;
            pathY[top] = ny;
        } else {
            top--;
        }
    }

    for(int y=0; y<maze_height; y++)
        for(int x=0; x<maze_width; x++)
            maze[y][x].visited = false;
}

void fillCell(int x, int y, int color)
{
    int px = x * cell_size + 1;
    int py = y * cell_size + 1;
    int size = cell_size - 1;

    setfillstyle(SOLID_FILL, color);
    bar(px, py, px + size, py + size);
}

bool solveMaze(int x, int y, int endX, int endY)
{
    if(x == endX && y == endY) {
        fillCell(x, y, GREEN);
        return true;
    }

    maze[y][x].visited = true;
    fillCell(x, y, YELLOW);
    delay(30);

    for(int i=0; i<4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if(nx < 0 || nx >= maze_width || ny < 0 || ny >= maze_height) continue;
        if(maze[ny][nx].visited) continue;
        if(i == 0 && maze[y][x].walls[0]) continue;
        if(i == 1 && maze[y][x].walls[1]) continue;
        if(i == 2 && maze[y][x].walls[2]) continue;
        if(i == 3 && maze[y][x].walls[3]) continue;

        if(solveMaze(nx, ny, endX, endY)) {
            fillCell(x, y, GREEN);
            return true;
        }
    }

    fillCell(x, y, RED);
    delay(20);
    return false;
}

int main()
{
    srand(time(nullptr));

    int win_width = maze_width * cell_size;
    int win_height = maze_height * cell_size;

    initwindow(win_width, win_height, "Maze Generator and Solver");
    setbkcolor(WHITE);
    cleardevice();

    generateMaze(0,0);
    drawMaze();

    solveMaze(0, 0, maze_width - 1, maze_height -1);

    delay(2000);
    closegraph();

    return 0;
}