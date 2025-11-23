#include "sort.h" 
#include <stdio.h>
#include <stdlib.h>

void menu() {
    printf("\n\n========= Menu =======\n\n");
    printf("[1] - Estatisticas\n"); 
    printf("[2] - BubbleSort\n");
    printf("[3] - MergeSort\n");
    printf("[4] - Mudar Colecao de Dados [Inverso ou Ordenado]\n"); 
    printf("[5] - Salvar Arquivo\n"); 
    printf("[0] - Sair\n");
}

void executarAcao(int escolha, int *colecao_atual, int arr_mestre[TAMANHO]) {
    int arr_teste[TAMANHO];
    int n_teste = 100;
    // Variáveis de Contagem 
    long long trocas_bubble = 0;
    long long comps_bubble = 0;
    long long movs_merge = 0;
    long long comps_merge = 0;
    
    switch (escolha) {
        case 1: // Estatístiacas
         printf("--- Estatisticas (A implementar) ---\n");
            printf("A ideia e colocar o tempo de execucao,\n Quantas execucoes foram feitas, e tals etals ");
            break;
        case 2: // Bubble Sort
         printf("--- Executando Bubble Sort ---\n");
            copiarArray(arr_mestre, arr_teste, TAMANHO);

            printf("Lista antes da ordenacao:\n");
            imprimir(arr_teste, n_teste);

            bubbleSort(arr_teste, TAMANHO,&trocas_bubble, &comps_bubble); 
            printf("Lista apos Bubble Sort:\n");
            imprimir(arr_teste, n_teste);
            printf("\n--- Contadores (N= %d) ---\n", TAMANHO);
            printf("Comparacoes: %lld\n", comps_bubble);
            printf("Trocas: %lld\n", trocas_bubble);
            


            break;
        case 3: // Merge Sort
            printf("--- Executando Merge Sort ---\n");
            copiarArray(arr_mestre, arr_teste, TAMANHO);
            printf("Lista antes da ordenacao:\n");
            imprimir(arr_teste, n_teste);
            mergeSort(arr_teste, 0, TAMANHO - 1, &movs_merge, &comps_merge); 
            printf("Lista apos Merge Sort:\n");
            imprimir(arr_teste, n_teste);
            printf("\n--- Contadores (N= %d) ---\n",TAMANHO);
            printf("Comparacoes: %lld\n", comps_merge);
            printf("Movimentacoes: %lld\n", movs_merge);
            break;
        case 4:
         printf("\n--- Selecao da Colecao de Dados ---\n");
            printf("[1] - 10.000 Elementos ORDENADOS (Caso Favoravel)\n");
            printf("[2] - 10.000 Elementos INVERSOS (Caso Desfavoravel)\n");
            
            escolha = lerInteiro("\nEscolha a colecao para testar: ");

            if (escolha == 1) {
                gerarArrayOrdenado(arr_mestre, TAMANHO);
                *colecao_atual = 1;
                printf("\nColecao ORDENADA gerada com sucesso!\n");
            } else if (escolha == 2) {
                gerarArrayInverso(arr_mestre, TAMANHO);
                *colecao_atual = 2;
                printf("\nColecao INVERSA gerada com sucesso!\n");
            } else {
                printf("\nOpcao invalida. Usando colecao ORDENADA por padrao.\n");
                gerarArrayOrdenado(arr_mestre, TAMANHO);
                *colecao_atual = 1;
            }
            break;
        case 5:
            printf("--- Digite qual resultado deseja salvar:  ---\n");
            break;
        default:
            printf("\nOpcao invalida (Erro Desconhecido)\n");
            break;
    }
}


int main() {
    int arr_mestre[TAMANHO];
    int colecao_atual = 0; // // Variável de Controle:(1=Ordenado, 2=Inverso)
    int n = sizeof(arr_mestre) / sizeof(arr_mestre[0]);
    int escolha;
    int voltar;

    gerarArrayOrdenado(arr_mestre, TAMANHO); // Gera a coleção standart
    colecao_atual = 1;

    while (1) {
        limparTela();
        menu();
        
        printf("\nDigite a opcao desejada: ");
        escolha = lerInteiro("");

        if (escolha == 0) {
            printf("\nFinalizando o programa... Obrigado pela atencao!");
            break;
        }

        // Se a opção for válida (1 a 4)
        if (escolha >= 1 && escolha <= 5) {
            int continuarNaOpcao = 1;
            while (continuarNaOpcao) {
                limparTela();
                executarAcao(escolha, &colecao_atual, arr_mestre);

                // Voltar ao Menu / Continuar / Sair
                int voltarValido = 0;
                do {
                    printf("\n\n[1]-Voltar ao Menu / [2]-Continuar (Repetir Acao) / [0]-Sair\n: ");
                    voltar = lerInteiro("");
                    if (voltar == 1 || voltar == 0 || voltar == 2) {
                        voltarValido = 1;
                    } else {
                        printf("\nDigito invalido: ");
                    }
                } while (!voltarValido);

                if (voltar == 1) { // Voltar ao Menu
                    continuarNaOpcao = 0; 
                } else if (voltar == 0) { // Sair
                    limparTela();
                    printf("\nFinalizando o programa... Obrigado pela atencao!");
                    return EXIT_SUCCESS; 
                } else { // voltar == 2: Continuar (Repetir Acao)
                    continuarNaOpcao = 1;
                }
            }
        } else {
            printf("\nOpcao invalida, por favor insira um numero entre 0 e 7.\n");
            printf("Pressione Enter para continuar...");
            // Limpa o buffer antes de pausar
            while (getchar() != '\n'); 
            getchar(); // Pausa para o usuário ver a mensagem de erro
        }
    }

    return EXIT_SUCCESS;
}