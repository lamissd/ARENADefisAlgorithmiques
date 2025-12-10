#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME 50
#define MAX_DEFI 50
#define SCORE_FILE "scores.txt"

typedef struct {
    char nom[MAX_NAME];
    char defi[MAX_DEFI];
    int score;
} ScoreEntry;


int calculerScore(int difficulte, int tentatives, int operations, int temps) {
    int score = difficulte;

    score -= tentatives * 2;      
    score -= operations / 5;      
    score -= temps / 3;

    if (score < 0) score = 0;
    return score;
}


void ajouterScore(const char *nom, const char *defi, int score) {
    FILE *f = fopen(SCORE_FILE, "a");
    if (!f) {
        printf("Erreur : impossible d'ouvrir le fichier des scores.\n");
        return;
    }
    fprintf(f, "%s;%s;%d\n", nom, defi, score);
    fclose(f);
}


void afficherClassement() {
    FILE *f = fopen(SCORE_FILE, "r");
    if (!f) {
        printf("\nAucun score enregistré.\n");
        return;
    }

    printf("\n===== CLASSEMENT =====\n");

    char nom[MAX_NAME], defi[MAX_DEFI];
    int score;

    while (fscanf(f, "%[^;];%[^;];%d\n", nom, defi, &score) == 3) {
        printf("%s — %s : %d pts\n", nom, defi, score);
    }

    fclose(f);
}


void soumissionSolution() {
    char nom[MAX_NAME];
    char defi[MAX_DEFI];
    int difficulte, tentatives, operations, temps;

    printf("\n=== Soumission d'une solution ===\n");

    printf("Votre nom : ");
    scanf("%s", nom);

    printf("Nom du défi : ");
    scanf("%s", defi);

    printf("Difficulté du défi : ");
    scanf("%d", &difficulte);

    printf("Nombre de tentatives : ");
    scanf("%d", &tentatives);

    printf("Nombre d'opérations effectuées : ");
    scanf("%d", &operations);

    printf("Temps (en secondes) : ");
    scanf("%d", &temps);

    int score = calculerScore(difficulte, tentatives, operations, temps);
    printf("\nScore obtenu : %d pts\n", score);

    ajouterScore(nom, defi, score);
}

int main() {
    int choix;

    do {
        printf("\n===== MENU ARENA =====\n");
        printf("1. Soumettre une solution\n");
        printf("2. Voir le classement\n");
        printf("3. Quitter\n");
        printf("Choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                soumissionSolution();
                break;
            case 2:
                afficherClassement();
                break;
            case 3:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }

    } while (choix != 3);

    return 0;
}
