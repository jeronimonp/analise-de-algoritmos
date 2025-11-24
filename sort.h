#ifndef SORT_H
#define SORT_H

#include <stddef.h> //  definição de tamanho, se necessário

// Definições de Constantes
#define TAMANHO 10000
// Número padrão de repetições para experimentos
#define REPETICOES 100

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


void calcularEstatisticas(double valores[], int n,
                          double *min, double *max,
                          double *media, double *moda,
                          double *desvioPadrao);

// Salvar resultados brutos (tempos por repetição) em CSV
void salvarResultadosCSV(const char *nomeArquivo,
                         double times_bubble[], double times_merge[], double times_hibrido[],
                         int n, int n0, int colecao_id);

// Timed wrappers: retornam tempo em segundos gasto na chamada
double bubbleSortTimed(int arr[], int tamanho, long long *trocas, long long *comparacoes);
double mergeSortTimed(int arr[], int esquerda, int direita, long long *movimentacoes, long long *comparacoes);
// Timed wrapper para mergeSortHibrido
double mergeSortHibridoTimed(int arr[], int esquerda, int direita, int n0, long long *movimentacoes, long long *comparacoes);

#endif // SORT_H