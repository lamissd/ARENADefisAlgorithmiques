#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void insertion_sort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void swap(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main() {
    int n = 10;
    int arr[10];

    srand(time(NULL));

    // Génère un tableau aléatoire
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100;

    printf("===== TRI ULTIME =====\n\n");
    printf("Voici ton tableau mélange :\n");
    print_array(arr, n);

    printf("\nChoisis ton arme :\n");
    printf("1️⃣ - Tri à bulles (Bubble Sort)\n");
    printf("2️⃣ - Tri par insertion (Insertion Sort)\n");
    printf("3️⃣ - Tri rapide (Quicksort)\n");
    printf("\n➡️ Ton choix : ");

    int choix;
    scanf("%d", &choix);

    printf("\nTu as choisi : ");
    if (choix == 1) printf("Tri à bulles !\n");
    else if (choix == 2) printf("Tri par insertion !\n");
    else printf("Tri rapide !\n");

    printf("\n🔄 Tri en cours...\n");

    // Applique le tri choisi
    if (choix == 1) bubble_sort(arr, n);
    else if (choix == 2) insertion_sort(arr, n);
    else quicksort(arr, 0, n - 1);

    printf("\n🎉 Résultat trié :\n");
    print_array(arr, n);

    printf("\nBravo ! Tu as terminé le défi Tri Ultime !\n");

    return 0;
}
