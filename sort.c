#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------------- MERGE SORT ---------------------

void merge(int arr[], int esquerda, int meio, int direita, long long *movimentacoes, long long *comparacoes) {
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    int *esqTemp = malloc(n1 * sizeof(int));
    int *dirTemp = malloc(n2 * sizeof(int));
    if (!esqTemp || !dirTemp) {
        fprintf(stderr, "Erro de alocacao em merge\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n1; i++) esqTemp[i] = arr[esquerda + i];
    for (int j = 0; j < n2; j++) dirTemp[j] = arr[meio + 1 + j];

    int i = 0, j = 0, k = esquerda;
    while (i < n1 && j < n2) {
        (*comparacoes)++;
        if (esqTemp[i] <= dirTemp[j]) {
            arr[k++] = esqTemp[i++];
        } else {
            arr[k++] = dirTemp[j++];
        }
        (*movimentacoes)++;
    }
    while (i < n1) {
        arr[k++] = esqTemp[i++];
        (*movimentacoes)++;
    }
    while (j < n2) {
        arr[k++] = dirTemp[j++];
        (*movimentacoes)++;
    }

    free(esqTemp);
    free(dirTemp);
}

void mergeSort(int arr[], int esquerda, int direita, long long *movimentacoes, long long *comparacoes) {
    if (esquerda >= direita) return;
    int meio = esquerda + (direita - esquerda) / 2;
    mergeSort(arr, esquerda, meio, movimentacoes, comparacoes);
    mergeSort(arr, meio + 1, direita, movimentacoes, comparacoes);
    merge(arr, esquerda, meio, direita, movimentacoes, comparacoes);
}

// ---------------------- BUBBLE SORT --------------------

void bubbleSort(int arr[], int tamanho, long long *trocas, long long *comparacoes) {
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = 0; j < tamanho - i - 1; j++) {
            (*comparacoes)++;
            if (arr[j] > arr[j + 1]) {
                (*trocas)++;
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// ------------------ BUBBLE PARA UM INTERVALO (Híbrido) -----------------

void bubbleSortParte(int arr[], int inicio, int fim) {
    if (inicio >= fim) return;
    int tamanho = fim - inicio + 1;
    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = inicio; j < inicio + tamanho - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// ------------------ MERGE SORT HÍBRIDO -----------------
// Usa bubbleSortParte quando subvetor <= n0
void mergeSortHibrido(int arr[], int esquerda, int direita, int n0) {
    if (esquerda >= direita) return;
    int tamanho = direita - esquerda + 1;
    if (tamanho <= n0) {
        bubbleSortParte(arr, esquerda, direita);
        return;
    }
    int meio = esquerda + (direita - esquerda) / 2;
    mergeSortHibrido(arr, esquerda, meio, n0);
    mergeSortHibrido(arr, meio + 1, direita, n0);
    long long mov = 0, comp = 0;
    merge(arr, esquerda, meio, direita, &mov, &comp);
}

// ------------------ Encontrar N0 (crossover) -----------------
// Estratégia:
// Testa n de 2 a MAX_N0; para cada candidato, executa REP_SMALL repetições
// em subarrays de tamanho n retirados de arr_mestre (com posições distintas)
// e compara tempo médio de bubble vs merge. Retornamos primeiro n com
// tempo_merge < tempo_bubble (crossover). Se não encontrar, retorna um n padrão.
int encontrarN0(int arr_mestre[], int tamanho) {
    const int MAX_N0 = 500; // pesquisa até 500 (ajustável)
    const int REP_SMALL = 8; // repetições por candidato
    double best_diff = 1e300;
    int best_n = 32; // fallback

    int *temp = malloc(tamanho * sizeof(int));
    if (!temp) return best_n;

    // Usar semente fixa para reprodutibilidade
    srand(0);

    for (int n = 2; n <= MAX_N0; n++) {
        double soma_bubble = 0.0;
        double soma_merge = 0.0;

        for (int r = 0; r < REP_SMALL; r++) {
            // escolher uma posição aleatória válida para subarray de tamanho n
            int start = rand() % (tamanho - n + 1);
            for (int i = 0; i < n; i++) temp[i] = arr_mestre[start + i];

            // medir bubble para tamanho n (copiar para local)
            int *tmp2 = malloc(n * sizeof(int));
            if (!tmp2) { free(temp); return best_n; }
            for (int i = 0; i < n; i++) tmp2[i] = temp[i];
            clock_t s = clock();
            long long trocas = 0, comps = 0;
            bubbleSort(tmp2, n, &trocas, &comps);
            clock_t e = clock();
            soma_bubble += (double)(e - s) / (double)CLOCKS_PER_SEC;
            free(tmp2);

            // medir merge para tamanho n
            tmp2 = malloc(n * sizeof(int));
            if (!tmp2) { free(temp); return best_n; }
            for (int i = 0; i < n; i++) tmp2[i] = temp[i];
            s = clock();
            long long mov = 0, comp = 0;
            mergeSort(tmp2, 0, n - 1, &mov, &comp);
            e = clock();
            soma_merge += (double)(e - s) / (double)CLOCKS_PER_SEC;
            free(tmp2);
        }

        double avg_b = soma_bubble / (double)REP_SMALL;
        double avg_m = soma_merge / (double)REP_SMALL;

        // diferença positiva significa merge mais rápido
        double diff = avg_b - avg_m;

        // se merge já for mais rápido (diff>0), encontramos crossover
        if (diff > 0.0) {
            free(temp);
            return n;
        }

        // track best (maior diferença negativa -> bubble ainda bem melhor)
        if (diff < best_diff) {
            best_diff = diff;
            best_n = n;
        }
    }

    free(temp);
    // fallback: se nenhum n fez merge mais rápido, retorna best_n
    return best_n;
}
