#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int v1;
    int v2;
    char op;
    int res;
} Etape;

Etape historique[10];
int solution_trouvee = 0;

void afficher_solution(int nb_etapes);

void chercher(int tab[], int n, int cible, int profondeur) {
    if (solution_trouvee) return;

    for (int i = 0; i < n; i++) {
        if (tab[i] == cible) {
            afficher_solution(profondeur);
            solution_trouvee = 1;
            return;
        }
    }

    if (n <= 1) return;

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int a = tab[i];
            int b = tab[j];

            int suivant[6];
            int idx = 0;
            for (int k = 0; k < n; k++) {
                if (k != i && k != j) {
                    suivant[idx++] = tab[k];
                }
            }

            Etape temp;
            temp.v1 = a;
            temp.v2 = b;
            temp.op = '+';
            temp.res = a + b;
            historique[profondeur] = temp;
            suivant[idx] = a + b;
            chercher(suivant, n - 1, cible, profondeur + 1);
            if (solution_trouvee) return;

            if (a != 1 && b != 1) {
                Etape temp;
                temp.v1 = a;
                temp.v2 = b;
                temp.op = '*';
                temp.res = a * b;
                historique[profondeur] = temp;
                suivant[idx] = a * b;
                chercher(suivant, n - 1, cible, profondeur + 1);
                if (solution_trouvee) return;
            }

            if (a != b) {
                int grand = (a > b) ? a : b;
                int petit = (a > b) ? b : a;
                Etape temp;
                temp.v1 = grand;
                temp.v2 = petit;
                temp.op = '-';
                temp.res = grand - petit;
                historique[profondeur] = temp;
                suivant[idx] = grand - petit;
                chercher(suivant, n - 1, cible, profondeur + 1);
                if (solution_trouvee) return;
            }

            if (b > 1 && a % b == 0) {
                Etape temp;
                temp.v1 = a;
                temp.v2 = b;
                temp.op = '/';
                temp.res = a / b;
                historique[profondeur] = temp;
                suivant[idx] = a / b;
                chercher(suivant, n - 1, cible, profondeur + 1);
                if (solution_trouvee) return;
            }
            if (a > 1 && b % a == 0) {
                Etape temp;
                temp.v1 = b;
                temp.v2 = a;
                temp.op = '/';
                temp.res = b / a;
                historique[profondeur] = temp;
                suivant[idx] = b / a;
                chercher(suivant, n - 1, cible, profondeur + 1);
                if (solution_trouvee) return;
            }
        }
    }
}

void afficher_solution(int nb_etapes) {
    printf("\n>>> SOLUTION TROUVEE en %d etapes :\n", nb_etapes);
    for (int i = 0; i < nb_etapes; i++) {
        printf("   %d.  %d %c %d = %d\n",
               i + 1,
               historique[i].v1,
               historique[i].op,
               historique[i].v2,
               historique[i].res);
    }
    printf("----------------------------------\n");
}

int main() {
    int nombres[6];
    int cible;
    int i;

    printf("==================================\n");
    printf("      JEU : LE COMPTE EST BON     \n");
    printf("==================================\n");

    printf("Veuillez entrer les 6 nombres disponibles :\n");
    for (i = 0; i < 6; i++) {
        printf(" -> Nombre %d : ", i + 1);
        scanf("%d", &nombres[i]);
    }

    printf("\nVeuillez entrer le nombre CIBLE a atteindre : ");
    scanf("%d", &cible);

    printf("\n--- VERIFICATION ---\n");
    printf("Nombres disponibles : { ");
    for (i = 0; i < 6; i++) {
        printf("%d ", nombres[i]);
    }
    printf("}\n");
    printf("Cible a atteindre   : %d\n", cible);
    printf("--------------------\n");
    
    printf("Recherche de solution en cours...\n");

    chercher(nombres, 6, cible, 0);

    if (!solution_trouvee) {
        printf("\nAucune solution exacte trouvee avec ces nombres.\n");
    }

    return 0;
}

