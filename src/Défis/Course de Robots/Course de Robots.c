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

int main() {
    printf("========================================\n");
    printf("       COURSE DE ROBOTS - LABYRINTHE\n");
    printf("========================================\n");
    
    afficherLabyrinthe(1, 1);
    
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
    
    int rx = 1, ry = 1; // Position de depart
    int ex = 5, ey = 5; // Position d'arrivee
    
    printf("\n========================================\n");
    printf("       EXECUTION DE VOTRE SEQUENCE\n");
    printf("========================================\n");
    
    int valide = 1;
    int longueur = strlen(sequence);
    
    for(int i = 0; i < longueur; i++) {
        char move = sequence[i];
        int nx = rx, ny = ry;
        
        if(move == 'N' || move == 'n') {
            nx = rx - 1;
            printf("\nMouvement %d: N (Nord)\n", i+1);
        }
        else if(move == 'S' || move == 's') {
            nx = rx + 1;
            printf("\nMouvement %d: S (Sud)\n", i+1);
        }
        else if(move == 'E' || move == 'e') {
            ny = ry + 1;
            printf("\nMouvement %d: E (Est)\n", i+1);
        }
        else if(move == 'O' || move == 'o') {
            ny = ry - 1;
            printf("\nMouvement %d: O (Ouest)\n", i+1);
        }
        else {
            printf("\nMouvement %d: '%c' INVALIDE!\n", i+1, move);
            valide = 0;
            break;
        }
        
        // Verifier si le mouvement est possible
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
    
    printf("\n========================================\n");
    printf("              RESULTAT\n");
    printf("========================================\n");
    
    if(valide) {
        if(rx == ex && ry == ey) {
            printf("\n*** VICTOIRE! ***\n");
            printf("Vous avez atteint l'arrivee en %d mouvements!\n", longueur);
            printf("Votre solution: %s\n", sequence);
        } else {
            printf("\n*** ECHEC! ***\n");
            printf("Vous n'avez pas atteint l'arrivee.\n");
            printf("Position finale: (%d, %d)\n", rx, ry);
            printf("Position cible: (%d, %d)\n", ex, ey);
        }
    } else {
        printf("\n*** SEQUENCE INVALIDE! ***\n");
        printf("La sequence contient des mouvements impossibles.\n");
    }
    
    printf("\n========================================\n");
    
    return 0;
}
}