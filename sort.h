#ifndef SORT_H
#define SORT_H

#include <stddef.h> //  definição de tamanho, se necessário

// Definições de Constantes
#define TAMANHO 10000

// Funções de Ordenação do Trabalho do Manu
void merge(int arr[], int esquerda, int meio, int direita, long long *movimentacoes, long long *comparacoes); // Núcleo
void mergeSort(int arr[], int esquerda, int direita, long long *movimentacoes, long long *comparacoes);

void bubbleSort(int arr[], int tamanho, long long *trocas, long long *comparacoes); 

// Protótipos para o híbrido (a ser implementado)
void bubbleSortParte(int arr[], int inicio, int fim); 
void mergeSortHibrido(int arr[], int esquerda, int direita, int n0);

// Protótipos das Funções Utilitárias
void limparTela();
int lerInteiro(const char *mensagem);
void imprimir(int arr[], int tamanho);
void copiarArray(int origem[], int destino[], int tamanho);
void gerarArrayOrdenado(int arr[], int tamanho);
void gerarArrayInverso(int arr[], int tamanho);


#endif // SORT_H