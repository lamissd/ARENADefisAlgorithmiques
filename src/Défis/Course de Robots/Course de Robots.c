#include <stdio.h>
#include <stdlib.h>

#define MAX 100

typedef struct {
    int x, y;
} Point;

char grid[MAX][MAX];
int visited[MAX][MAX];
Point parent[MAX][MAX];

Point queue[MAX * MAX];
int front = 0, back = 0;

int rows, cols;
int start_x, start_y;
int end_x, end_y;

void enqueue(Point p) {
    queue[back++] = p;
}

Point dequeue() {
    return queue[front++];
}

int isEmpty() {
    return front == back;
}

void loadGrid(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Erreur ouverture fichier\n");
        exit(1);
    }

    rows = 0;
    while (fgets(grid[rows], MAX, f)) {
        rows++;
    }
    fclose(f);

    cols = 0;
    while (grid[0][cols] != '\n' && grid[0][cols] != '\0')
        cols++;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == 'S') {
                start_x = i;
                start_y = j;
            }
            if (grid[i][j] == 'E') {
                end_x = i;
                end_y = j;
            }
        }
}

void BFS() {
    enqueue((Point){start_x, start_y});
    visited[start_x][start_y] = 1;

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!isEmpty()) {
        Point p = dequeue();

        if (p.x == end_x && p.y == end_y)
            return; 

        for (int i = 0; i < 4; i++) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if (nx >= 0 && nx < rows && ny >= 0 && ny < cols &&
                grid[nx][ny] != '#' &&
                !visited[nx][ny]) {

                visited[nx][ny] = 1;
                parent[nx][ny] = p;
                enqueue((Point){nx, ny});
            }
        }
    }
}

void reconstructPath() {
    Point path[MAX * MAX];
    int len = 0;

    Point curr = (Point){end_x, end_y};

    while (!(curr.x == start_x && curr.y == start_y)) {
        path[len++] = curr;
        curr = parent[curr.x][curr.y];
    }

    path[len++] = (Point){start_x, start_y};

    printf("\nChemin trouvé :\n");

    for (int i = len - 1; i >= 0; i--) {
        printf("(%d,%d) ", path[i].x, path[i].y);
    }
    printf("\n");

    printf("\nSéquence de mouvements : ");

    for (int i = len - 1; i > 0; i--) {
        Point a = path[i];
        Point b = path[i-1];

        if (b.x == a.x + 1) printf("S ");
        else if (b.x == a.x - 1) printf("N ");
        else if (b.y == a.y + 1) printf("E ");
        else if (b.y == a.y - 1) printf("O ");
    }
    printf("\n");
}

int main() {
    loadGrid("maze.txt");

    BFS();
    reconstructPath();

    return 0;
}
