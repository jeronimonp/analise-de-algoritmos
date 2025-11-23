#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void menu() {
    printf("\n\n========= Menu =======\n\n");
    printf("[1] - Executar Experimento Completo (ordenado + inverso)\n");
    printf("[2] - Gerar Colecao Ordenada\n");
    printf("[3] - Gerar Colecao Inversa\n");
    printf("[4] - Salvar ultimos resultados (CSV)\n");
    printf("[0] - Sair\n");
}

// Estrutura para guardar ultimos resultados (para salvar/exibir)
typedef struct {
    int colecao; // 1 ordenada, 2 inversa
    int n0;
    double times_bubble[REPETICOES];
    double times_merge[REPETICOES];
    double times_hibrido[REPETICOES];
    int valid;
} Results;

Results last_results_ordered = {0};
Results last_results_inversed = {0};

void executarExperimentoParaColecao(int arr_mestre[], int colecao_id, Results *store) {
    printf("\n--- Iniciando experimento para colecao: %s ---\n", (colecao_id == 1) ? "ORDENADA" : "INVERSA");

    // 1) Encontrar n0 baseado nessa colecao
    printf("Determinando n0 (crossover) para esta colecao. Aguarde...\n");
    int n0 = encontrarN0(arr_mestre, TAMANHO);
    printf("n0 encontrado: %d\n", n0);
    store->n0 = n0;

    int copia[TAMANHO];
    // 2) Executar 100 vezes Bubble e Merge e Hibrido
    for (int i = 0; i < REPETICOES; i++) {
        // Bubble
        copiarArray(arr_mestre, copia, TAMANHO);
        clock_t s = clock();
        long long trocas = 0, comps = 0;
        bubbleSort(copia, TAMANHO, &trocas, &comps);
        clock_t e = clock();
        store->times_bubble[i] = (double)(e - s) / (double)CLOCKS_PER_SEC;

        // Merge
        copiarArray(arr_mestre, copia, TAMANHO);
        s = clock();
        long long mov = 0, comp = 0;
        mergeSort(copia, 0, TAMANHO - 1, &mov, &comp);
        e = clock();
        store->times_merge[i] = (double)(e - s) / (double)CLOCKS_PER_SEC;

        // Hibrido (usa n0)
        copiarArray(arr_mestre, copia, TAMANHO);
        s = clock();
        mergeSortHibrido(copia, 0, TAMANHO - 1, n0);
        e = clock();
        store->times_hibrido[i] = (double)(e - s) / (double)CLOCKS_PER_SEC;

        if ((i + 1) % 10 == 0) {
            printf("Repeticoes: %d/%d\n", i + 1, REPETICOES);
        }
    }

    store->colecao = colecao_id;
    store->valid = 1;

    // Estatísticas e impressão resumida
    double min, max, media, moda, desvio;
    printf("\nResumo estatístico (colecao %s):\n", (colecao_id == 1) ? "ORDENADA" : "INVERSA");

    printf("\n-- Bubble --\n");
    calcularEstatisticas(store->times_bubble, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("\n-- Merge --\n");
    calcularEstatisticas(store->times_merge, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("\n-- Hibrido (n0=%d) --\n", n0);
    calcularEstatisticas(store->times_hibrido, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("\nExperimento concluido para esta colecao.\n");
}

int main() {
    int arr_mestre[TAMANHO];
    int colecao_atual = 1; // por padrão ordenada
    gerarArrayOrdenado(arr_mestre, TAMANHO);

    while (1) {
        limparTela();
        menu();
        int opc = lerInteiro("\nEscolha a opcao: ");

        if (opc == 0) {
            printf("Encerrando...\n");
            break;
        }

        if (opc == 1) {
            // Executar experimento completo: ordenada e inversa
            // Ordenada
            gerarArrayOrdenado(arr_mestre, TAMANHO);
            executarExperimentoParaColecao(arr_mestre, 1, &last_results_ordered);
            // Inversa
            gerarArrayInverso(arr_mestre, TAMANHO);
            executarExperimentoParaColecao(arr_mestre, 2, &last_results_inversed);
            printf("\nExperimento completo (ambas colecoes) finalizado.\n");
            printf("Use opcao 4 para salvar os CSVs (ultimos resultados)\n");
            lerInteiro("Pressione Enter (digite 0) para voltar ao menu: ");
        } else if (opc == 2) {
            gerarArrayOrdenado(arr_mestre, TAMANHO);
            printf("Colecao ordenada gerada.\n");
            lerInteiro("Pressione Enter (digite 0) para voltar ao menu: ");
        } else if (opc == 3) {
            gerarArrayInverso(arr_mestre, TAMANHO);
            printf("Colecao inversa gerada.\n");
            lerInteiro("Pressione Enter (digite 0) para voltar ao menu: ");
        } else if (opc == 4) {
            // Salvar últimos resultados (se existirem)
            if (last_results_ordered.valid) {
                salvarResultadosCSV("resultados_ordenado.csv",
                                    last_results_ordered.times_bubble,
                                    last_results_ordered.times_merge,
                                    last_results_ordered.times_hibrido,
                                    REPETICOES);
            } else {
                printf("Nao ha resultados para colecao ordenada.\n");
            }
            if (last_results_inversed.valid) {
                salvarResultadosCSV("resultados_inverso.csv",
                                    last_results_inversed.times_bubble,
                                    last_results_inversed.times_merge,
                                    last_results_inversed.times_hibrido,
                                    REPETICOES);
            } else {
                printf("Nao ha resultados para colecao inversa.\n");
            }
            lerInteiro("Pressione Enter (digite 0) para voltar ao menu: ");
        } else {
            printf("Opcao invalida.\n");
            lerInteiro("Pressione Enter (digite 0) para voltar ao menu: ");
        }
    }

    return 0;
}
