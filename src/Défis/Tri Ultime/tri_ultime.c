#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    unsigned long long comparisons;
    unsigned long long swaps;
    int aborted; 
} Stats;

typedef struct {
    unsigned long long max_comparisons; 
    unsigned long long max_swaps;
} Limits;

long long ms_now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (long long)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

static inline void check_limits(Stats *s, Limits *lim) {
    if (!s || !lim) return;
    if ((lim->max_comparisons > 0 && s->comparisons > lim->max_comparisons) ||
        (lim->max_swaps > 0 && s->swaps > lim->max_swaps)) {
        s->aborted = 1;
    }
}

void bubble_sort(int arr[], int n, Stats *s, Limits *lim) {
    if (n <= 1) return;
    for (int i = 0; i < n - 1 && !s->aborted; i++) {
        int swapped = 0;
        for (int j = 0; j < n - 1 - i && !s->aborted; j++) {
            s->comparisons++;
            if (arr[j] > arr[j+1]) {
                int tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                s->swaps++;
                swapped = 1;
            }
            check_limits(s, lim);
        }
        if (!swapped) break;
    }
}

void insertion_sort(int arr[], int n, Stats *s, Limits *lim) {
    if (n <= 1) return;
    for (int i = 1; i < n && !s->aborted; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0) {
            s->comparisons++;
            if (arr[j] > key) {
                arr[j+1] = arr[j];
                s->swaps++; 
                j--;
                check_limits(s, lim);
                if (s->aborted) return;
            } else {
                break;
            }
        }
        arr[j+1] = key;
       
    }
}

int partition_lomuto(int arr[], int lo, int hi, Stats *s, Limits *lim) {
    int pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j <= hi - 1 && !s->aborted; j++) {
        s->comparisons++;
        if (arr[j] <= pivot) {
            i++;
            if (i != j) {
                int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
                s->swaps++;
            }
        }
        check_limits(s, lim);
    }
    if (!s->aborted) {
        if (i+1 != hi) {
            int tmp = arr[i+1]; arr[i+1] = arr[hi]; arr[hi] = tmp;
            s->swaps++;
        }
    }
    return i + 1;
}

void quicksort_rec(int arr[], int lo, int hi, Stats *s, Limits *lim) {
    if (lo < hi && !s->aborted) {
        int p = partition_lomuto(arr, lo, hi, s, lim);
        if (s->aborted) return;
        quicksort_rec(arr, lo, p - 1, s, lim);
        if (s->aborted) return;
        quicksort_rec(arr, p + 1, hi, s, lim);
    }
}

void quick_sort(int arr[], int n, Stats *s, Limits *lim) {
    quicksort_rec(arr, 0, n - 1, s, lim);
}

int *clone_array(int arr[], int n) {
    int *b = malloc(sizeof(int) * n);
    if (!b) { perror("malloc"); exit(1); }
    memcpy(b, arr, sizeof(int) * n);
    return b;
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if (i) printf(" ");
        printf("%d", arr[i]);
    }
    printf("\n");
}

int is_sorted(int arr[], int n) {
    for (int i = 1; i < n; i++) if (arr[i-1] > arr[i]) return 0;
    return 1;
}

void prompt_array(int **arr_out, int *n_out) {
    int n;
    printf("Entrer la taille du tableau (n <= 100000) : ");
    if (scanf("%d", &n) != 1) { printf("Entrée invalide\n"); exit(1); }
    if (n <= 0 || n > 100000) { printf("Taille invalide\n"); exit(1); }

    int *arr = malloc(sizeof(int) * n);
    if (!arr) { perror("malloc"); exit(1); }

    printf("Souhaitez-vous :\n 1) Entrer les éléments manuellement\n 2) Générer aléatoirement\nChoix (1/2) : ");
    int c; if (scanf("%d", &c) != 1) exit(1);

    if (c == 1) {
        printf("Entrer %d entiers séparés par espace ou newline:\n", n);
        for (int i = 0; i < n; i++) {
            if (scanf("%d", &arr[i]) != 1) { printf("Entrée invalide\n"); exit(1); }
        }
    } else {
        printf("Entrer seed (0 pour time) : ");
        unsigned int seed; if (scanf("%u", &seed) != 1) exit(1);
        if (seed == 0) seed = (unsigned int)time(NULL);
        srand(seed);
        for (int i = 0; i < n; i++) arr[i] = rand() % 100000;
        printf("Tableau généré avec seed=%u\n", seed);
    }
    *arr_out = arr;
    *n_out = n;
}

Limits prompt_limits() {
    Limits lim = {0,0};
    printf("Limiter le nombre de comparaisons ? (0 = non) : ");
    if (scanf("%llu", &lim.max_comparisons) != 1) exit(1);
    printf("Limiter le nombre de swaps ? (0 = non) : ");
    if (scanf("%llu", &lim.max_swaps) != 1) exit(1);
    return lim;
}

int choose_algorithm() {
    printf("Choisir l'algorithme :\n");
    printf("1) Bubble Sort\n");
    printf("2) Insertion Sort\n");
    printf("3) QuickSort (Lomuto)\n");
    printf("4) Tester les 3 algos (comparaison)\n");
    printf("Choix : ");
    int a; if (scanf("%d", &a) != 1) exit(1);
    return a;
}

void run_sort_and_report(int arr[], int n, int algo, Limits lim) {
    int *copy = clone_array(arr, n);
    Stats s = {0,0,0};
    long long t0 = ms_now();

    if (algo == 1) {
        bubble_sort(copy, n, &s, &lim);
    } else if (algo == 2) {
        insertion_sort(copy, n, &s, &lim);
    } else if (algo == 3) {
        quick_sort(copy, n, &s, &lim);
    } else {
        free(copy); return;
    }

    long long t1 = ms_now();

    printf("\n--- Résultat (%s) ---\n",
        algo==1 ? "Bubble" : algo==2 ? "Insertion" : "QuickSort");
    printf("Temps (ms) : %lld\n", t1 - t0);
    printf("Comparaisons : %llu\n", s.comparisons);
    printf("Swaps : %llu\n", s.swaps);
    printf("Interrompu par limite : %s\n", s.aborted ? "OUI" : "NON");
    printf("Tri effectif (is_sorted) : %s\n", is_sorted(copy, n) ? "OUI" : "NON");

    if (n <= 100) {
        printf("Tableau trié : ");
        print_array(copy, n);
    } else {
        printf("(tableau trop grand pour affichage complet)\n");
    }

    free(copy);
}

int main() {
    int *arr = NULL, n = 0;
    prompt_array(&arr, &n);

    Limits lim = prompt_limits();
    int algo = choose_algorithm();

    if (algo >=1 && algo <=3) {
        run_sort_and_report(arr, n, algo, lim);
    } else if (algo == 4) {
        for (int a = 1; a <= 3; a++) {
            run_sort_and_report(arr, n, a, lim);
        }
    } else {
        printf("Choix invalide\n");
    }

    free(arr);
    return 0;
}
