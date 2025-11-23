#ifndef SORT_H
#define SORT_H

#include <stddef.h>

#define TAMANHO 10000
#define REPETICOES 100

// Ordenação
void merge(int arr[], int esquerda, int meio, int direita, long long *movimentacoes, long long *comparacoes);
void mergeSort(int arr[], int esquerda, int direita, long long *movimentacoes, long long *comparacoes);

void bubbleSort(int arr[], int tamanho, long long *trocas, long long *comparacoes);

// Híbrido
void bubbleSortParte(int arr[], int inicio, int fim);
void mergeSortHibrido(int arr[], int esquerda, int direita, int n0);

// Utilitários e I/O
void limparTela();
int lerInteiro(const char *mensagem);
void imprimir(int arr[], int tamanho);
void copiarArray(int origem[], int destino[], int tamanho);
void gerarArrayOrdenado(int arr[], int tamanho);
void gerarArrayInverso(int arr[], int tamanho);

// Estatísticas e salvamento
void calcularEstatisticas(double valores[], int n,
                          double *min, double *max,
                          double *media, double *moda,
                          double *desvioPadrao);

void salvarResultadosCSV(const char *nomeArquivo,
                         double times_bubble[], double times_merge[], double times_hibrido[],
                         int n);

// Determinar N0 (crossover)
int encontrarN0(int arr_mestre[], int tamanho);

#endif // SORT_H
