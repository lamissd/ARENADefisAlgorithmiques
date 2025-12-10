#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_CODE 4
#define MAX_VALEUR 9
#define MAX_ESSAIS 10

typedef struct {
    int bien_places;
    int mal_places;
} Resultat;

void generer_secret(int secret[]) {
    srand(time(NULL));
    for (int i = 0; i < TAILLE_CODE; i++) {
        secret[i] = (rand() % MAX_VALEUR) + 1;
    }
}

Resultat evaluer_essai(int secret[], int essai[]) {
    Resultat res = {0, 0};
    int secret_utilise[TAILLE_CODE] = {0};
    int essai_utilise[TAILLE_CODE] = {0};

    for (int i = 0; i < TAILLE_CODE; i++) {
        if (secret[i] == essai[i]) {
            res.bien_places++;
            secret_utilise[i] = 1;
            essai_utilise[i] = 1;
        }
    }

    for (int i = 0; i < TAILLE_CODE; i++) {
        if (!essai_utilise[i]) {
            for (int j = 0; j < TAILLE_CODE; j++) {
                if (!secret_utilise[j] && essai[i] == secret[j]) {
                    res.mal_places++;
                    secret_utilise[j] = 1;
                    break;
                }
            }
        }
    }

    return res;
}

void afficher_fin(int secret[], int gagne, int nb_tours) {
    printf("\n=======================================\n");
    if (gagne) {
        printf("       BRAVO ! VOUS AVEZ GAGNE EN %d ESSAIS !\n", nb_tours);
    } else {
        printf("       ECHEC ! PLUS D'ESSAIS DISPONIBLES\n");
    }
    printf("La sequence secrete etait : ");
    for (int i = 0; i < TAILLE_CODE; i++) {
        printf("%d ", secret[i]);
    }
    printf("\n=======================================\n");
}

int main() {
    int secret[TAILLE_CODE];
    int essai[TAILLE_CODE];
    int tours = 0;
    int gagne = 0;
    Resultat score;

    printf("=======================================\n");
    printf("       MASTERMIND ALGORITHMIQUE        \n");
    printf("=======================================\n");
    printf("Devinez la sequence de %d chiffres (entre 1 et %d).\n", TAILLE_CODE, MAX_VALEUR);
    printf("Vous avez %d essais.\n\n", MAX_ESSAIS);

    generer_secret(secret);

    while (tours < MAX_ESSAIS && !gagne) {
        tours++;
        printf("--- Essai %d / %d ---\n", tours, MAX_ESSAIS);
        printf("Entrez %d chiffres separes par des espaces : ", TAILLE_CODE);

        for (int i = 0; i < TAILLE_CODE; i++) {
            scanf("%d", &essai[i]);
            if (essai[i] < 1) essai[i] = 1;
            if (essai[i] > MAX_VALEUR) essai[i] = MAX_VALEUR;
        }

        score = evaluer_essai(secret, essai);

        if (score.bien_places == TAILLE_CODE) {
            gagne = 1;
        } else {
            printf("Resultat : %d Bien place(s) | %d Mal place(s)\n\n",
                   score.bien_places, score.mal_places);
        }
    }

    afficher_fin(secret, gagne, tours);

    if (gagne) return tours;
    return 0;
}
