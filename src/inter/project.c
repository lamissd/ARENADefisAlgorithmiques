#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define SCORE_FILE "scores.txt"
#define MAX_SCORES 200


#define N_DIJKSTRA 6
#define INF_DIST 99999
#define SIZE_LABY 7
#define TAILLE_CODE 4
#define MAX_VALEUR 9
#define MAX_ESSAIS 10
#define MAX_NUMBERS_COMPTE 6


typedef struct
{
    char nom[30];
    char defi[30];
    int score;
    double temps;
    int essais;
    int difficulte; 
} ScoreEntry;


typedef struct { int x, y; } NodeLaby;

typedef struct {
    int bien_places;
    int mal_places;
} ResultatMastermind;

typedef struct {
    int v1;
    int v2;
    char op;
    int res;
} EtapeCompte;


EtapeCompte historique_compte[10];

char lab[SIZE_LABY][SIZE_LABY] = {
    {'#','#','#','#','#','#','#'},
    {'#','S',' ',' ','#',' ','#'},
    {'#',' ','#',' ','#',' ','#'},
    {'#',' ',' ',' ',' ',' ','#'},
    {'#','#',' ','#','#',' ','#'},
    {'#',' ',' ',' ',' ','E','#'},
    {'#','#','#','#','#','#','#'}
};



int calculScore(double temps, int essais, int difficulte)
{
    int score = difficulte * 1000 - (temps * 5) - (essais * 10);
    if (score < 0)
        score = 0;
    return score;
}

int chargerScores(ScoreEntry tab[], int max)
{
    FILE *f = fopen(SCORE_FILE, "r");
    if (!f)
        return 0;

    int count = 0;
    while (count < max && fscanf(f, "%s %s %d %lf %d %d",
                                 tab[count].nom,
                                 tab[count].defi,
                                 &tab[count].score,
                                 &tab[count].temps,
                                 &tab[count].essais,
                                 &tab[count].difficulte) == 6) 
    {
        count++;
    }

    fclose(f);
    return count;
}

void saveScore(char nom[], char defi[], double temps, int essais, int difficulte)
{
    FILE *f = fopen(SCORE_FILE, "a");
    if (!f)
    {
        printf("Erreur: impossible d'ouvrir %s\n", SCORE_FILE);
        return;
    }

    int score = calculScore(temps, essais, difficulte);

    fprintf(f, "%s %s %d %.2f %d %d\n", nom, defi, score, temps, essais, difficulte);

    fclose(f);
    printf("\n Score sauvegardé avec succès ! (Score: %d)\n", score);
}

void afficherScores()
{
    FILE *f = fopen(SCORE_FILE, "r");
    if (!f)
    {
        printf("Aucun score trouvé.\n");
        return;
    }

    char nom[30], defi[30];
    int score, essais, difficulte;
    double temps;

    printf("\n===== CLASSEMENT BRUT =====\n");
    printf("Nom      | Défi      | Score | Temps (s) | Essais | Diff\n");
    printf("---------|-----------|-------|-----------|--------|-----\n");


    while (fscanf(f, "%s %s %d %lf %d %d", nom, defi, &score, &temps, &essais, &difficulte) == 6)
    {
        printf("%-8s | %-9s | %5d | %9.2f | %6d | %4d\n",
               nom, defi, score, temps, essais, difficulte);
    }

    fclose(f);
}

void trierScores(ScoreEntry tab[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            if (tab[j].score > tab[i].score)
            {
                ScoreEntry tmp = tab[i];
                tab[i] = tab[j];
                tab[j] = tmp;
            }
        }
    }
}

void afficherMeilleursScores()
{
    ScoreEntry scores[MAX_SCORES];
    int n = chargerScores(scores, MAX_SCORES);

    if (n == 0)
    {
        printf("\nAucun score trouvé.\n");
        return;
    }

    trierScores(scores, n);

    printf("\n===== MEILLEURS SCORES (TRIÉS) =====\n");

    for (int i = 0; i < n && i < 10; i++) 
    {
        printf("%2d) %-8s | Défi: %-9s | Score: %5d | Temps: %.2f s | Essais: %d\n",
               i + 1,
               scores[i].nom,
               scores[i].defi,
               scores[i].score,
               scores[i].temps,
               scores[i].essais);
    }
}



void afficher_solution_compte(int nb_etapes) {
    for (int i = 0; i < nb_etapes; i++) {
        printf("%d %c %d = %d\n",
               historique_compte[i].v1, historique_compte[i].op,
               historique_compte[i].v2, historique_compte[i].res);
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
            int suivant[MAX_NUMBERS_COMPTE];
            int idx = 0;
            for (int k = 0; k < n; k++)
                if (k != i && k != j) suivant[idx++] = tab[k];

            EtapeCompte temp;
            int r;

            temp.v1 = a; temp.v2 = b; temp.op = '+'; temp.res = a + b;
            historique_compte[profondeur] = temp;
            suivant[idx] = a + b;
            r = chercher(suivant, n - 1, cible, profondeur + 1);
            if (r != -1) return r;

            if (a != 1 && b != 1) {
                temp.v1 = a; temp.v2 = b; temp.op = '*'; temp.res = a * b;
                historique_compte[profondeur] = temp;
                suivant[idx] = a * b;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }

            if (a != b) {
                int g = (a > b) ? a : b;
                int p = (a > b) ? b : a;
                if (g - p > 0) {
                    temp.v1 = g; temp.v2 = p; temp.op = '-'; temp.res = g - p;
                    historique_compte[profondeur] = temp;
                    suivant[idx] = g - p;
                    r = chercher(suivant, n - 1, cible, profondeur + 1);
                    if (r != -1) return r;
                }
            }

            if (b > 1 && a % b == 0) {
                temp.v1 = a; temp.v2 = b; temp.op = '/'; temp.res = a / b;
                historique_compte[profondeur] = temp;
                suivant[idx] = a / b;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }

            if (a > 1 && b % a == 0 && a != b) {
                temp.v1 = b; temp.v2 = a; temp.op = '/'; temp.res = b / a;
                historique_compte[profondeur] = temp;
                suivant[idx] = b / a;
                r = chercher(suivant, n - 1, cible, profondeur + 1);
                if (r != -1) return r;
            }
        }
    }

    return -1;
}

void get_random_compte_est_bon_problem(int *nombres, int *cible) {
    int petites_plaques[] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10};
    int grandes_plaques[] = {25, 50, 75, 100};
    
    for (int i = 0; i < MAX_NUMBERS_COMPTE; i++) {
        if (i < 4 || rand() % 3 == 0) {
            nombres[i] = petites_plaques[rand() % 20];
        } else {
            nombres[i] = grandes_plaques[rand() % 4];
        }
    }
    
    *cible = (rand() % (999 - 100 + 1)) + 100;
}

int run_defi_compte_est_bon() {
    printf("\n========================================\n");
    printf("💡 DÉFI : LE COMPTE EST BON (DFS)\n");
    printf("========================================\n");

    int nombres[MAX_NUMBERS_COMPTE];
    int cible;

    get_random_compte_est_bon_problem(nombres, &cible);

    printf("CIBLE: %d\n", cible);
    printf("Nombres: ");
    for (int i = 0; i < MAX_NUMBERS_COMPTE; i++) {
        printf("%d ", nombres[i]);
    }
    printf("\n");
    
    int steps_solution = chercher(nombres, MAX_NUMBERS_COMPTE, cible, 0);

    if (steps_solution != -1) {
        printf("\nSolution trouvee en %d etapes (votre code) :\n", steps_solution);
        afficher_solution_compte(steps_solution);
        return 1;
    } else {
        printf("\nAucune solution exacte trouvee.\n");
        return 0;
    }
}




void afficherLabyrinthe(int rx, int ry) {
    printf("\n");
    for(int i = 0; i < SIZE_LABY; i++) {
        for(int j = 0; j < SIZE_LABY; j++) {
            if(i == rx && j == ry)
                printf("R ");
            else
                printf("%c ", lab[i][j]);
        }
        printf("\n");
    }
}

int bfsCheminLePlusCourt(int sx, int sy, int ex, int ey) {
    int visited[SIZE_LABY][SIZE_LABY] = {0};
    int dist[SIZE_LABY][SIZE_LABY];
    NodeLaby queue[SIZE_LABY * SIZE_LABY];
    int front = 0, back = 0;

    for(int i = 0; i < SIZE_LABY; i++)
        for(int j = 0; j < SIZE_LABY; j++)
            dist[i][j] = 9999;

    queue[back++] = (NodeLaby){sx, sy};
    dist[sx][sy] = 0;
    visited[sx][sy] = 1;

    int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}}; 

    while(front < back) {
        NodeLaby cur = queue[front++];
        if(cur.x == ex && cur.y == ey) return dist[cur.x][cur.y];

        for(int d = 0; d < 4; d++) {
            int nx = cur.x + dirs[d][0];
            int ny = cur.y + dirs[d][1];

            if(nx >= 0 && nx < SIZE_LABY && ny >= 0 && ny < SIZE_LABY &&
               !visited[nx][ny] && lab[nx][ny] != '#') {
                visited[nx][ny] = 1;
                dist[nx][ny] = dist[cur.x][cur.y] + 1;
                queue[back++] = (NodeLaby){nx, ny};
            }
        }
    }
    return -1; 
}

int run_defi_robot_labyrinthe() {
    printf("\n========================================\n");
    printf("🤖 DÉFI : COURSE DE ROBOTS - LABYRINTHE (BFS)\n");
    printf("========================================\n");

    int rx = 1, ry = 1; 
    int ex = 5, ey = 5; 
    
    afficherLabyrinthe(rx, ry);

    printf("\nObjectif: Deplacer le robot R vers E (N, S, E, O)\n");
    printf("Entrez votre sequence (Exemple: EESSEEO): ");

    char sequence[100];
    if (scanf("%s", sequence) != 1) return 9999;

    int shortest = bfsCheminLePlusCourt(rx, ry, ex, ey);
    printf("\nLongueur du plus court chemin (optimal) : %d mouvements\n", shortest);

    int valide = 1;
    int longueur = strlen(sequence);
    
    for(int i = 0; i < longueur; i++) {
        char move = sequence[i];
        int nx = rx, ny = ry;

        if(move == 'N' || move == 'n') nx--;
        else if(move == 'S' || move == 's') nx++;
        else if(move == 'E' || move == 'e') ny++;
        else if(move == 'O' || move == 'o') ny--;
        else { valide = 0; break; }

        if(nx < 0 || nx >= SIZE_LABY || ny < 0 || ny >= SIZE_LABY || lab[nx][ny] == '#') {
            valide = 0; 
            break; 
        }

        rx = nx;
        ry = ny;
    }
    
    if(valide && rx == ex && ry == ey) {
        printf("\n*** ARRIVÉE ATTEINTE en %d mouvements ***\n", longueur);
        return longueur; 
    } else {
        printf("\n*** ECHEC ! Non atteint ou sequence invalide. ***\n");
        return 9999;
    }
}



void generer_secret(int secret[]) {
    for (int i = 0; i < TAILLE_CODE; i++) {
        secret[i] = (rand() % MAX_VALEUR) + 1;
    }
}

ResultatMastermind evaluer_essai(int secret[], int essai[]) {
    ResultatMastermind res = {0, 0};
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

int run_defi_mastermind() {
    printf("\n=======================================\n");
    printf("🧠 DÉFI : MASTERMIND ALGORITHMIQUE\n");
    printf("=======================================\n");
    printf("Devinez la sequence de %d chiffres (1 a %d) en %d essais max.\n\n", 
           TAILLE_CODE, MAX_VALEUR, MAX_ESSAIS);

    int secret[TAILLE_CODE];
    int essai[TAILLE_CODE];
    int tours = 0;
    int gagne = 0;
    ResultatMastermind score_mm;

    generer_secret(secret);

    while (tours < MAX_ESSAIS && !gagne) {
        tours++;
        printf("--- Essai %d / %d ---\n", tours, MAX_ESSAIS);
        printf("Entrez %d chiffres separes par des espaces : ", TAILLE_CODE);

        for (int i = 0; i < TAILLE_CODE; i++) {
            if (scanf("%d", &essai[i]) != 1) { 
                printf("Erreur de lecture. Fin de l'essai.\n");
                return MAX_ESSAIS + 1;
            }
        }

        score_mm = evaluer_essai(secret, essai);

        if (score_mm.bien_places == TAILLE_CODE) {
            gagne = 1;
        } else {
            printf("Resultat : %d Bien place(s) | %d Mal place(s)\n\n",
                   score_mm.bien_places, score_mm.mal_places);
        }
    }

    if (gagne) {
        printf("\n BRAVO ! VOUS AVEZ GAGNE EN %d ESSAIS !\n", tours);
        return tours; 
    } else {
        printf("\n ECHEC ! Plus d'essais. La sequence etait : ");
        for (int i = 0; i < TAILLE_CODE; i++) printf("%d ", secret[i]);
        printf("\n");
        return MAX_ESSAIS + 1; 
    }
}



void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        int p = i + 1;
        
        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int run_defi_tri_ultime() {
    printf("\n========================================\n");
    printf("🔢 DÉFI : TRI ULTIME (Quicksort)\n");
    printf("========================================\n");
    
    int n = 10;
    int arr[10];

    for (int i = 0; i < n; i++) arr[i] = rand() % 100;

    printf("Voici votre tableau melange :\n");
    print_array(arr, n);

    printf("\nExecution du Quicksort...\n");
    quicksort(arr, 0, n - 1);

    printf("\n🎉 Resultat trie :\n");
    print_array(arr, n);
    
    return 1; 
}



void print_graph_dijkstra(int g[N_DIJKSTRA][N_DIJKSTRA]) {
    printf("Graphe pondere (INF=' . '):\n\n");
    for(int i=0;i<N_DIJKSTRA;i++){
        for(int j=0;j<N_DIJKSTRA;j++)
            if(g[i][j] == INF_DIST) printf(" . ");
            else printf("%2d ", g[i][j]);
        printf("\n");
    }
}

void dijkstra(int g[N_DIJKSTRA][N_DIJKSTRA], int start, int dist[]) {
    int visited[N_DIJKSTRA];
    for(int i=0;i<N_DIJKSTRA;i++){ dist[i]=INF_DIST; visited[i]=0; }

    dist[start] = 0;

    for(int count=0; count<N_DIJKSTRA-1; count++) {
        int min = INF_DIST, u = -1;

        for(int i=0;i<N_DIJKSTRA;i++)
            if(!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        
        if (u == -1) break; 
        visited[u] = 1;

        for(int v=0; v<N_DIJKSTRA; v++)
            if(!visited[v] && g[u][v] != INF_DIST && 
               dist[u] != INF_DIST && dist[u] + g[u][v] < dist[v])
                dist[v] = dist[u] + g[u][v];
    }
}

int cost_path(int g[N_DIJKSTRA][N_DIJKSTRA], int path[], int len) {
    int cost = 0;
    if (len < 2) return INF_DIST; 

    for(int i=0; i<len-1; i++) {
        if(path[i] < 0 || path[i] >= N_DIJKSTRA || path[i+1] < 0 || path[i+1] >= N_DIJKSTRA)
             return INF_DIST;

        if(g[path[i]][path[i+1]] == INF_DIST)
            return INF_DIST;
        else
            cost += g[path[i]][path[i+1]];
    }
    return cost;
}

int run_defi_dijkstra() {
    printf("\n========================================\n");
    printf("📊 DÉFI : PLUS COURT CHEMIN (DIJKSTRA)\n");
    printf("========================================\n");
    
    int g[N_DIJKSTRA][N_DIJKSTRA];

    for(int i=0;i<N_DIJKSTRA;i++)
        for(int j=0;j<N_DIJKSTRA;j++)
            if(i==j) g[i][j]=0;
            else g[i][j] = (rand()%3==0)? (rand()%9+1) : INF_DIST; 

    print_graph_dijkstra(g);

    int start = rand()%N_DIJKSTRA;
    int end = rand()%N_DIJKSTRA;
    while(end==start) end = rand()%N_DIJKSTRA;

    printf("\nTrouvez le plus court chemin entre le Sommet %d et le Sommet %d\n", start, end);
    printf("Entrez votre chemin (sequence de sommets, terminer par -1). Doit commencer par %d et finir par %d :\n", start, end);

    int path[20], len = 0;
    int temp_input;

    while(len < 20) {
        if (scanf("%d", &temp_input) != 1) { 
            while (getchar() != '\n');
            printf("Erreur de lecture. Retourne 0 essais.\n");
            return 0; 
        }
        if(temp_input == -1) break;
        path[len++] = temp_input;
    }

    if (len == 0 || path[0] != start || path[len-1] != end) {
        printf("\n❌ Chemin incomplet ou incorrect.\n");
        return 0; 
    }
    
    int player_cost = cost_path(g, path, len);
    
    printf("\n--- RESULTAT ---\n");

    if(player_cost == INF_DIST) {
        printf("❌ Chemin impossible.\n");
        return 0;
    }

    printf("Votre cout : %d\n", player_cost);

    return player_cost; 
}



void jouerDefi(int choix)
{
    char nom[30];
    printf("\nEntrez votre nom : ");
    while (getchar() != '\n');
    if (scanf("%29s", nom) != 1) {
        strcpy(nom, "Joueur");
    }

    int essais_ou_performance = 0;
    int difficulte = 0;
    char nomDefi[30];

    clock_t start = clock();

    switch (choix)
    {
    case 1:
        strcpy(nomDefi, "CompteBon");
        difficulte = 9;
        essais_ou_performance = run_defi_compte_est_bon(); 
        if (essais_ou_performance == 0) difficulte = 0; 
        essais_ou_performance = 1;
        break;
    case 2:
        strcpy(nomDefi, "Robot");
        difficulte = 7;
        essais_ou_performance = run_defi_robot_labyrinthe(); 
        if (essais_ou_performance == 9999) difficulte = 0;
        break;
    case 3:
        strcpy(nomDefi, "Mastermind");
        difficulte = 6;
        essais_ou_performance = run_defi_mastermind();
        if (essais_ou_performance > MAX_ESSAIS) difficulte = 0;
        break;
    case 4:
        strcpy(nomDefi, "TriUltime");
        difficulte = 3; 
        essais_ou_performance = run_defi_tri_ultime();
        if (essais_ou_performance == 0) difficulte = 0;
        essais_ou_performance = 1; 
        break;
    case 5:
        strcpy(nomDefi, "Dijkstra");
        difficulte = 8;
        essais_ou_performance = run_defi_dijkstra(); 
        if (essais_ou_performance == 0) difficulte = 0;
        break;
    default:
        printf("Choix incorrect.\n");
        return;
    }

    clock_t end = clock();
    double temps = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\n Temps utilise : %.2f secondes\n", temps);

    saveScore(nom, nomDefi, temps, essais_ou_performance, difficulte);
}


void menu()
{
    int choix;

    do
    {
        printf("\n====== ARENA DE DÉFIS (MENU PRINCIPAL) ======\n");
        printf("1 - Jouer : Le Compte est Bon (DFS)\n");
        printf("2 - Jouer : Course Robot (Labyrinthe / BFS)\n");
        printf("3 - Jouer : Mastermind (Logique)\n");
        printf("4 - Jouer : Tri Ultime (Quicksort)\n");
        printf("5 - Jouer : Plus Court Chemin (Dijkstra)\n");
        printf("6 - Afficher tous les scores\n");
        printf("7 - Afficher les meilleurs scores (tries)\n");
        printf("8 - Quitter\n");
        printf("Votre choix : ");

        if (scanf("%d", &choix) != 1) {
             while (getchar() != '\n');
             printf("Entree invalide.\n");
             choix = 0;
             continue;
        }

        if (choix >= 1 && choix <= 5)
            jouerDefi(choix);
        else if (choix == 6)
            afficherScores();
        else if (choix == 7)
            afficherMeilleursScores();
        else if (choix == 8)
            printf("\nAu revoir et bonne chance pour votre projet !\n");
        else
            printf("Choix invalide.\n");

    } while (choix != 8);
}


int main()
{
    srand(time(NULL));
    menu();
    return 0;
}