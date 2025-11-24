#include "sort.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------------------- MERGE SORT ---------------------

void merge(int arr[], int esquerda, int meio, int direita, long long *movimentacoes, long long *comparacoes) {
    
    // Calcular os tamanhos
    int n1 = meio - esquerda + 1;
    int n2 = direita - meio;

    // Alocar arrays temporários (Esquerda e Direita)
    int * esqTemp = malloc(n1 * sizeof(int));
    int * dirTemp = malloc(n2 * sizeof(int));

    // Copiar dados para os arrays temporários
    for (int i = 0; i < n1; i++)
        esqTemp[i] = arr[esquerda + i];
    for (int j = 0; j < n2; j++)
        dirTemp[j] = arr[meio + 1 + j];

    int i = 0; // indice do array esqTemp
    int j = 0; // indice do array dirTemp
    int k = esquerda; // indice do array principal

    while (i < n1 && j < n2) {

        (*comparacoes)++;

        if (esqTemp[i] <= dirTemp[j]) {
            arr[k] = esqTemp[i];
            i++;
        } else {
            arr[k] = dirTemp[j];
            j++;
        }
        (*movimentacoes)++;
        k++;
    }

    // Copiar o restante de Esq[], se houver
    while (i < n1) {
        arr[k] = esqTemp[i];
        (*movimentacoes)++;
        i++;
        k++;
    }

    // Copiar o restante de Dir[], se houver
    while (j < n2) {
        arr[k] = dirTemp[j];
        (*movimentacoes)++;
        j++;
        k++;
    }

    free(esqTemp);
    free(dirTemp);
}

void mergeSort(int arr[], int esquerda, int direita,long long *movimentacoes, long long *comparacoes) {
    if (esquerda >= direita) {
        return;
    }

    // Encontrar o meio
    int meio = esquerda + (direita - esquerda) / 2;


    mergeSort(arr, esquerda, meio, movimentacoes, comparacoes);
    mergeSort(arr, meio + 1, direita, movimentacoes, comparacoes);
    merge(arr, esquerda, meio, direita, movimentacoes, comparacoes);
}

// ---------------------- BUBBLE SORT --------------------

void bubbleSort(int arr[], int tamanho, long long *trocas, long long *comparacoes) {
    
    for (int i = 0; i < tamanho - 1; i++) { // percorre todos os elementos da lista
        
        // O loop vai até 'tamanho - i - 1' pois o resto estará ajustado no fim.
        for (int j = 0; j < tamanho - i - 1; j++) {

            (*comparacoes)++;
            
            // Se o elemento atual for maior que o próximo, então eles irão trocar de lugar
            if (arr[j] > arr[j + 1]) {
                
                (*trocas)++;
                // Realiza a troca usando uma variável temporária
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Bubble sort para um segmento do array (inicio..fim inclusive)
void bubbleSortParte(int arr[], int inicio, int fim) {
    int tamanho = fim - inicio + 1;
    if (tamanho <= 1) return;

    for (int i = 0; i < tamanho - 1; i++) {
        for (int j = inicio; j < inicio + tamanho - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Merge sort híbrido: usa bubbleSortParte quando o tamanho do subarray <= n0
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
    // Reuse existing merge but we don't have counters here; pass dummy pointers
    long long dummyMov = 0, dummyComp = 0;
    merge(arr, esquerda, meio, direita, &dummyMov, &dummyComp);
}

// Timed wrapper para bubbleSort — retorna tempo em segundos
double bubbleSortTimed(int arr[], int tamanho, long long *trocas, long long *comparacoes) {
    if (tamanho <= 0) return 0.0;

    // Copia do array original para poder repetir a ordenacao em uma copia
    int *orig = malloc(tamanho * sizeof(int));
    int *work = malloc(tamanho * sizeof(int));
    if (!orig || !work) {
        if (orig) free(orig);
        if (work) free(work);
        return 0.0;
    }
    for (int i = 0; i < tamanho; i++) orig[i] = arr[i];

    double total = 0.0;
    int repeats = 1;
    const double minTime = 0.01; // 10ms
    const int maxRepeats = 1024;

    while (1) {
        clock_t ini = clock();
        for (int r = 0; r < repeats; r++) {
            // copia o original para work e ordena a copia
            for (int i = 0; i < tamanho; i++) work[i] = orig[i];
            long long tmpTrocas = 0, tmpComps = 0;
            bubbleSort(work, tamanho, &tmpTrocas, &tmpComps);
        }
        clock_t fim = clock();
        total = (double)(fim - ini) / CLOCKS_PER_SEC;

        if (total >= minTime || repeats >= maxRepeats) break;
        repeats *= 2;
    }

    // copia o resultado ordenado da ultima repeticao para arr
    for (int i = 0; i < tamanho; i++) arr[i] = work[i];

    free(orig);
    free(work);
    return total / repeats;
}

// Timed wrapper para mergeSort — retorna tempo em segundos
double mergeSortTimed(int arr[], int esquerda, int direita, long long *movimentacoes, long long *comparacoes) {
    int tamanho = direita - esquerda + 1;
    if (tamanho <= 0) return 0.0;

    int *orig = malloc(tamanho * sizeof(int));
    int *work = malloc(tamanho * sizeof(int));
    if (!orig || !work) {
        if (orig) free(orig);
        if (work) free(work);
        return 0.0;
    }
    for (int i = 0; i < tamanho; i++) orig[i] = arr[esquerda + i];

    double total = 0.0;
    int repeats = 1;
    const double minTime = 0.01; // 10ms
    const int maxRepeats = 1024;

    while (1) {
        clock_t ini = clock();
        for (int r = 0; r < repeats; r++) {
            for (int i = 0; i < tamanho; i++) work[i] = orig[i];
            long long tmpMov = 0, tmpComp = 0;
            mergeSort(work, 0, tamanho - 1, &tmpMov, &tmpComp);
        }
        clock_t fim = clock();
        total = (double)(fim - ini) / CLOCKS_PER_SEC;

        if (total >= minTime || repeats >= maxRepeats) break;
        repeats *= 2;
    }

    // copia o resultado ordenado da ultima repeticao para arr[esquerda..direita]
    for (int i = 0; i < tamanho; i++) arr[esquerda + i] = work[i];

    free(orig);
    free(work);
    return total / repeats;
}

// Timed wrapper for hybrid mergeSort (uses bubble for small segments)
double mergeSortHibridoTimed(int arr[], int esquerda, int direita, int n0, long long *movimentacoes, long long *comparacoes) {
    int tamanho = direita - esquerda + 1;
    if (tamanho <= 0) return 0.0;

    int *orig = malloc(tamanho * sizeof(int));
    int *work = malloc(tamanho * sizeof(int));
    if (!orig || !work) {
        if (orig) free(orig);
        if (work) free(work);
        return 0.0;
    }
    for (int i = 0; i < tamanho; i++) orig[i] = arr[esquerda + i];

    double total = 0.0;
    int repeats = 1;
    const double minTime = 0.01; // 10ms
    const int maxRepeats = 1024;

    while (1) {
        clock_t ini = clock();
        for (int r = 0; r < repeats; r++) {
            for (int i = 0; i < tamanho; i++) work[i] = orig[i];
            // call hybrid on work array (indices 0..tamanho-1)
            mergeSortHibrido(work, 0, tamanho - 1, n0);
        }
        clock_t fim = clock();
        total = (double)(fim - ini) / CLOCKS_PER_SEC;

        if (total >= minTime || repeats >= maxRepeats) break;
        repeats *= 2;
    }

    // copy last result back to arr
    for (int i = 0; i < tamanho; i++) arr[esquerda + i] = work[i];

    free(orig);
    free(work);
    return total / repeats;
}