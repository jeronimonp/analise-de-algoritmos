#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int lerInteiro(const char *mensagem) {
    int valorInserido;
    char c;
    printf("%s", mensagem);
    while (1) {
        if (scanf("%d", &valorInserido) == 1) {
            c = getchar();
            if (c == '\n' || c == EOF) {
                return valorInserido;
            } else {
                printf("Entrada Invalida. Digite somente valores inteiros: ");
                while ((c = getchar()) != '\n' && c != EOF);
            }
        } else {
            printf("Entrada Invalida. Digite somente valores inteiros: ");
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

void imprimir(int arr[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", arr[i]);
        if (i < tamanho - 1) printf(", ");
    }
    printf("]\n");
}

void copiarArray(int origem[], int destino[], int tamanho) {
    for (int i = 0; i < tamanho; i++) destino[i] = origem[i];
}

void gerarArrayOrdenado(int arr[], int tamanho) {
    if (!arr || tamanho <= 0) return;
    for (int i = 0; i < tamanho; i++) arr[i] = i;
}

void gerarArrayInverso(int arr[], int tamanho) {
    if (!arr || tamanho <= 0) return;
    for (int i = 0; i < tamanho; i++) arr[i] = tamanho - 1 - i;
}

// -------------------- Estatísticas --------------------
void calcularEstatisticas(double valores[], int n, double *min, double *max, double *media, double *moda, double *desvio) {
    if (n <= 0) {
        *min = *max = *media = *moda = *desvio = 0.0;
        return;
    }

    *min = valores[0];
    *max = valores[0];
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        if (valores[i] < *min) *min = valores[i];
        if (valores[i] > *max) *max = valores[i];
        soma += valores[i];
    }
    *media = soma / (double)n;

    // Moda: agrupamento por tolerância (para doubles)
    double eps = 1e-9;
    double best_val = valores[0];
    int best_cnt = 1;
    for (int i = 0; i < n; i++) {
        int cnt = 1;
        for (int j = i + 1; j < n; j++) {
            if (fabs(valores[i] - valores[j]) <= eps) cnt++;
        }
        if (cnt > best_cnt) {
            best_cnt = cnt;
            best_val = valores[i];
        }
    }
    *moda = best_val;

    if (n == 1) {
        *desvio = 0.0;
        return;
    }

    double soma_q = 0.0;
    for (int i = 0; i < n; i++) {
        double d = valores[i] - *media;
        soma_q += d * d;
    }
    *desvio = sqrt(soma_q / (double)(n - 1)); // amostral
}

// Salvar CSV: colunas exec, bubble, merge, hibrido
void salvarResultadosCSV(const char *nomeArquivo, double times_bubble[], double times_merge[], double times_hibrido[], int n) {
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) {
        printf("Erro ao abrir arquivo para escrita: %s\n", nomeArquivo);
        return;
    }
    fprintf(f, "exec,tempo_bubble_s,tempo_merge_s,tempo_hibrido_s\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%.9f,%.9f,%.9f\n", i + 1, times_bubble[i], times_merge[i], times_hibrido[i]);
    }
    fclose(f);
    printf("Dados salvos em %s\n", nomeArquivo);
}
