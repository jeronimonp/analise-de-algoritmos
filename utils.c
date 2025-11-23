#include "sort.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
            // Limpa o buffer de entrada.
            c = getchar();
            if (c == '\n' || c == EOF) {
                return valorInserido;
            } else {
                printf("Entrada Invalida. Digite somente valores inteiros: ");
                // Limpa o resto da linha (buffer)
                while ((c = getchar()) != '\n' && c != EOF);
            }
        } else {
            printf("Entrada Invalida. Digite somente valores inteiros: ");
            // Limpa o buffer para o proximo scanf
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}



// ---------------------- FUNÇÕES DE MENU E AÇÃO ---------------------
void imprimir(int arr[], int tamanho) {
    printf("[");
    for (int i = 0; i < tamanho; i++) {
        printf("%d", arr[i]);
        if (i < tamanho - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}


// Implementar Geração de Dados e Cópia 


void copiarArray(int origem[], int destino[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        destino[i] = origem[i];
    }
}

void gerarArrayOrdenado(int arr[], int tamanho){
    
    if (arr == NULL || tamanho <= 0)
        return; 

    for (int i = 0; i < tamanho; i++){
          arr[i] = i;
    }
    

}

void gerarArrayInverso(int arr[], int tamanho){

    if (arr == NULL)
        return; 

    for (int i = 0; i < tamanho; i++){
          arr[i] = tamanho - 1 - i;
    }

}