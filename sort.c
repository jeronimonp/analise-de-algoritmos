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