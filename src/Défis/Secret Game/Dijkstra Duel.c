#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define N 6
#define INF 99999

void print_graph(int g[N][N]) {
    printf("Graphe pondéré :\n\n");

    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++)
            if(g[i][j] == INF) printf(" . ");
            else printf("%2d ", g[i][j]);
        printf("\n");
    }
}

void dijkstra(int g[N][N], int start, int dist[]) {
    int visited[N];
    for(int i=0;i<N;i++){ dist[i]=INF; visited[i]=0; }

    dist[start] = 0;

    for(int count=0; count<N-1; count++) {
        int min = INF, u = -1;

        for(int i=0;i<N;i++)
            if(!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }

        visited[u] = 1;

        for(int v=0; v<N; v++)
            if(!visited[v] && g[u][v] != INF && 
               dist[u] + g[u][v] < dist[v])
                dist[v] = dist[u] + g[u][v];
    }
}

int cost_path(int g[N][N], int path[], int len) {
    int cost = 0;

    for(int i=0; i<len-1; i++)
        if(g[path[i]][path[i+1]] == INF)
            return INF;

        else
            cost += g[path[i]][path[i+1]];

    return cost;
}

int main() {
    srand(time(NULL));

    int g[N][N];
    
    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++)
            if(i==j) g[i][j]=0;
            else g[i][j] = (rand()%3==0)? (rand()%9+1) : INF;

    print_graph(g);

    int start = rand()%N;
    int end   = rand()%N;

    while(end==start) end = rand()%N;

    printf("\nTrouve le plus court chemin entre %d et %d\n", start, end);

    int dist[N];
    dijkstra(g, start, dist);

    printf("Entre ton chemin (terminer par -1) :\n");

    int path[20], len = 0;

    while(1){
        scanf("%d", &path[len]);
        if(path[len] == -1) break;
        len++;
    }

    int player = cost_path(g, path, len);
    int optimal = dist[end];

    printf("\nRésultat :\n");

    if(player == INF)
        printf("❌ Chemin impossible.\n");

    else {
        printf("Votre cout : %d\n", player);
        printf("Cout optimal : %d\n", optimal);

        if(player == optimal)
            printf("🎉 Parfait ! Chemin optimal.\n");

        else
            printf("✅ Chemin valide mais pas optimal.\n");
    }

    return 0;
}
