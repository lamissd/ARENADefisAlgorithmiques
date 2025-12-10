#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v1;
    int v2;
    char op;
    int res;
} Etape;

Etape historique[10];

void afficher_solution(int nb_etapes) {
    for (int i = 0; i < nb_etapes; i++) {
        printf("%d %c %d = %d\n",
               historique[i].v1,
               historique[i].op,
               historique[i].v2,
               historique[i].res);
    }
}

int chercher(int tab[], int n, int cible, int profondeur) {
    for (int i = 0; i < n; i++) {
        if (tab[i] == cible) return profondeur;
    }

    if (n <= 1) return -1;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {

            int a = tab[i];
            int b = tab[j];

            int suivant[6];
            int idx = 0;
            for (int k = 0; k < n; k++)
                if (k != i && k != j) suivant[idx++] = tab[k];

            Etape temp;
            int r;

            temp.v1 = a; temp.v2 = b; temp.op = '+'; temp.res = a + b;
            historique[profondeur] = temp;
            suivant[idx] = a + b;
            r = chercher(suivant, n - 1, cible, profondeur + 1);
            if (r != -1) return r;

            if (a != 1 && b != 1) {
                temp.v1 = a; temp.v2 = b; temp.op = '*'; temp.res = a * b;
                historique[profondeur] = temp;
                suivant[idx] = a * b;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }

            if (a != b) {
                int g = (a > b) ? a : b;
                int p = (a > b) ? b : a;
                temp.v1 = g; temp.v2 = p; temp.op = '-'; temp.res = g - p;
                historique[profondeur] = temp;
                suivant[idx] = g - p;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }

            if (b > 1 && a % b == 0) {
                temp.v1 = a; temp.v2 = b; temp.op = '/'; temp.res = a / b;
                historique[profondeur] = temp;
                suivant[idx] = a / b;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }

            if (a > 1 && b % a == 0) {
                temp.v1 = b; temp.v2 = a; temp.op = '/'; temp.res = b / a;
                historique[profondeur] = temp;
                suivant[idx] = b / a;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }
        }
    }

    return -1;
}

int main() {
    int nombres[6];
    int cible;

    for (int i = 0; i < 6; i++)
        scanf("%d", &nombres[i]);

    scanf("%d", &cible);

    int steps = chercher(nombres, 6, cible, 0);

    return steps;
}
