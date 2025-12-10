#include <stdio.h>
#include <string.h>

#define SIZE 7

char lab[SIZE][SIZE] = {
    {'#','#','#','#','#','#','#'},
    {'#','S',' ',' ','#',' ','#'},
    {'#',' ','#',' ','#',' ','#'},
    {'#',' ',' ',' ',' ',' ','#'},
    {'#','#',' ','#','#',' ','#'},
    {'#',' ',' ',' ',' ','E','#'},
    {'#','#','#','#','#','#','#'}
};

typedef struct {
    int x, y;
} Node;

int bfsCheminLePlusCourt(int sx, int sy, int ex, int ey) {
    int visited[SIZE][SIZE] = {0};
    int dist[SIZE][SIZE];
    Node queue[SIZE * SIZE];
    int front = 0, back = 0;

    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            dist[i][j] = 9999;

    queue[back++] = (Node){sx, sy};
    dist[sx][sy] = 0;
    visited[sx][sy] = 1;

    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

    while(front < back) {
        Node cur = queue[front++];

        if(cur.x == ex && cur.y == ey)
            return dist[cur.x][cur.y];

        for(int d = 0; d < 4; d++) {
            int nx = cur.x + dirs[d][0];
            int ny = cur.y + dirs[d][1];

            if(nx >= 0 && nx < SIZE && ny >= 0 && ny < SIZE &&
               !visited[nx][ny] && lab[nx][ny] != '#') {

                visited[nx][ny] = 1;
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                queue[back++] = (Node){nx, ny};
            }
        }
    }
    return -1; 
}

// ----------------------------------------------------------------------
void afficherLabyrinthe(int rx, int ry) {
    printf("\n");
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            if(i == rx && j == ry)
                printf("R ");
            else
                printf("%c ", lab[i][j]);
        }
        printf("\n");
    }
}

// ----------------------------------------------------------------------
int main() {

    printf("========================================\n");
    printf("       COURSE DE ROBOTS - LABYRINTHE\n");
    printf("========================================\n");

    int rx = 1, ry = 1; // départ
    int ex = 5, ey = 5; // arrivée

    afficherLabyrinthe(rx, ry);

    printf("\nObjectif: Deplacer le robot R vers E\n");
    printf("\nEntrez votre sequence de mouvements:\n");
    printf("N = Nord (haut)\n");
    printf("S = Sud (bas)\n");
    printf("E = Est (droite)\n");
    printf("O = Ouest (gauche)\n");
    printf("\nExemple: EESSEEO\n");
    printf("\nVotre sequence: ");

    char sequence[100];
    scanf("%s", sequence);

    int shortest = bfsCheminLePlusCourt(1, 1, ex, ey);
    printf("\nLongueur du plus court chemin (optimal) : %d mouvements\n", shortest);

    printf("\n========================================\n");
    printf("       EXECUTION DE VOTRE SEQUENCE\n");
    printf("========================================\n");

    int valide = 1;
    int longueur = strlen(sequence);

    for(int i = 0; i < longueur; i++) {
        char move = sequence[i];
        int nx = rx, ny = ry;

        if(move == 'N' || move == 'n') {
            nx--;
            printf("\nMouvement %d: N (Nord)\n", i+1);
        }
        else if(move == 'S' || move == 's') {
            nx++;
            printf("\nMouvement %d: S (Sud)\n", i+1);
        }
        else if(move == 'E' || move == 'e') {
            ny++;
            printf("\nMouvement %d: E (Est)\n", i+1);
        }
        else if(move == 'O' || move == 'o') {
            ny--;
            printf("\nMouvement %d: O (Ouest)\n", i+1);
        }
        else {
            printf("\nMouvement %d: '%c' INVALIDE!\n", i+1, move);
            valide = 0;
            break;
        }

        if(nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
            printf("ERREUR: Hors des limites!\n");
            valide = 0;
            break;
        }

        if(lab[nx][ny] == '#') {
            printf("ERREUR: Collision avec un mur!\n");
            valide = 0;
            break;
        }

        rx = nx;
        ry = ny;

        printf("Position: (%d, %d)\n", rx, ry);
        afficherLabyrinthe(rx, ry);
    }

    // ---------------------------------------------------------------
    // Résultats
    // ---------------------------------------------------------------
    printf("\n========================================\n");
    printf("              RESULTAT\n");
    printf("========================================\n");

    if(valide) {
        if(rx == ex && ry == ey) {

            printf("\n*** ARRIVÉE ATTEINTE ! ***\n");
            printf("Votre solution : %s (%d mouvements)\n", sequence, longueur);

            if(longueur == shortest) {
                printf("\n🎉 VICTOIRE ABSOLUE ! 🎉\n");
                printf("Vous avez trouvé le chemin LE PLUS COURT !\n");
            } else {
                printf("\nVous avez atteint l'arrivée, mais pas avec le chemin optimal.\n");
                printf("Chemin optimal = %d mouvements\n", shortest);
                printf("Votre chemin = %d mouvements\n", longueur);
            }
        }
        else {
            printf("\n*** ECHEC ! Vous n'avez pas atteint l'arrivée. ***\n");
            printf("Position finale: (%d, %d)\n", rx, ry);
        }
    }
    else {
        printf("\n*** SEQUENCE INVALIDE ! ***\n");
        printf("La sequence contient des mouvements impossibles.\n");
    }

    printf("\n========================================\n");

    return 0;
}
