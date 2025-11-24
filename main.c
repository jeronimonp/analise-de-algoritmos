#include "sort.h" 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Protótipo para a função que determina N0
int determinarN0(int arr_mestre[], int maxN);
int gerarRelatorioCSV(int arr_mestre[], int maxN, const char *filename);
// Executa experimento completo (ordenado + inverso) e salva 100 repetições por algoritmo
void executarExperimentoCompleto(int arr_mestre[TAMANHO]);

void menu() {
    printf("\n\n========= Menu =======\n\n");
    printf("[1] - Estatisticas\n"); 
    printf("[2] - BubbleSort\n");
    printf("[3] - MergeSort\n");
    printf("[4] - Mudar Colecao de Dados [Inverso ou Ordenado]\n"); 
    printf("[5] - Salvar Arquivo\n"); 
    printf("[6] - Gerar Relatorio CSV (modo automatico)\n");
    printf("[7] - Executar Experimento Completo (ordenado + inverso)\n");
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
         // Estatístiacas
            printf("--- Estatisticas (100 execucoes) ---\n");

            double tempos_bubble[100];
            double tempos_merge[100];

            int copia[TAMANHO];

            // ---- 100 execuções Bubble ----
            for (int i = 0; i < 100; i++) {
                copiarArray(arr_mestre, copia, TAMANHO);

                long long troc = 0, comp = 0;
                double t = bubbleSortTimed(copia, TAMANHO, &troc, &comp);
                printf("Tempo BubbleSort (exec %d): %.9f s\n", i+1, t);
                tempos_bubble[i] = t;
            }

            // ---- 100 execuções Merge ----
            for (int i = 0; i < 100; i++) {
                copiarArray(arr_mestre, copia, TAMANHO);

                long long mov = 0, comp = 0;
                double tm = mergeSortTimed(copia, 0, TAMANHO - 1, &mov, &comp);
                tempos_merge[i] = tm;
            }

            // ---- Estatísticas completas ----
            double min, max, media, moda, desvio;

            printf("\n=== Estatisticas BubbleSort ===\n");
            calcularEstatisticas(tempos_bubble, 100, &min, &max, &media, &moda, &desvio);
            printf("Min: %.9f\nMax: %.9f\nMedia: %.9f\nModa: %.9f\nDesvio Padrao: %.9f\n",
                    min, max, media, moda, desvio);

            printf("\n=== Estatisticas MergeSort ===\n");
            calcularEstatisticas(tempos_merge, 100, &min, &max, &media, &moda, &desvio);
            printf("Min: %.9f\nMax: %.9f\nMedia: %.9f\nModa: %.9f\nDesvio Padrao: %.9f\n",
                    min, max, media, moda, desvio);

            // Determinar N0 (pode demorar dependendo do TAMANHO)
            printf("\nDeterminando N0 (comparando medias de tempo)...\n");
            int n0 = determinarN0(arr_mestre, TAMANHO);
            if (n0 > 0)
                printf("N0 determinado: %d (maior n onde BubbleSort <= MergeSort)\n", n0);
            else
                printf("Nao foi possivel determinar um N0 (bubble nao foi mais rapido em nenhum n testado)\n");

            break;
        case 2: // Bubble Sort
         printf("--- Executando Bubble Sort ---\n");
            copiarArray(arr_mestre, arr_teste, TAMANHO);

            printf("Lista antes da ordenacao:\n");
            imprimir(arr_teste, n_teste);

            double t_b = bubbleSortTimed(arr_teste, TAMANHO, &trocas_bubble, &comps_bubble);
            printf("Lista apos Bubble Sort:\n");
            imprimir(arr_teste, n_teste);
            printf("\n--- Contadores (N= %d) ---\n", TAMANHO);
            printf("Tempo total BubbleSort: %.9f s\n", t_b);
            printf("Comparacoes: %lld\n", comps_bubble);
            printf("Trocas: %lld\n", trocas_bubble);
            


            break;
        case 3: // Merge Sort
            printf("--- Executando Merge Sort ---\n");
            copiarArray(arr_mestre, arr_teste, TAMANHO);
            printf("Lista antes da ordenacao:\n");
            imprimir(arr_teste, n_teste);
            double t_m = mergeSortTimed(arr_teste, 0, TAMANHO - 1, &movs_merge, &comps_merge);
            printf("Lista apos Merge Sort:\n");
            imprimir(arr_teste, n_teste);
            printf("\n--- Contadores (N= %d) ---\n",TAMANHO);
            printf("Tempo total MergeSort: %.9f s\n", t_m);
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
        case 6:
            printf("--- Gerar relatorio CSV (modo automatico) ---\n");
            {
                char filename[256];
                printf("Nome do arquivo CSV (padrao: report.csv): ");
                if (scanf("%255s", filename) != 1) {
                    // usar padrao
                    strcpy(filename, "report.csv");
                }
                // Limpa resto do buffer
                int c; while ((c = getchar()) != '\n' && c != EOF);

                printf("Gerando relatorio em '%s' (isso pode demorar)...\n", filename);
                int ok = gerarRelatorioCSV(arr_mestre, TAMANHO, filename);
                if (ok) printf("Relatorio gerado com sucesso: %s\n", filename);
                else printf("Falha ao gerar relatorio. Verifique permissoes\n");
            }
            break;
        case 7:
            executarExperimentoCompleto(arr_mestre);
            break;
        default:
            printf("\nOpcao invalida (Erro Desconhecido)\n");
            break;
    }
}


int main(int argc, char *argv[]) {
    int arr_mestre[TAMANHO];
    int colecao_atual = 0; // // Variável de Controle:(1=Ordenado, 2=Inverso)
    int n = sizeof(arr_mestre) / sizeof(arr_mestre[0]);
    int escolha;
    int voltar;

    gerarArrayOrdenado(arr_mestre, TAMANHO); // Gera a coleção standart
    colecao_atual = 1;

    // Modo não-interativo: --auto-report executa experimento completo e sai
    if (argc > 1 && strcmp(argv[1], "--auto-report") == 0) {
        printf("Modo auto-report ativo: executando experimento completo (ordenada + inverso)\n");
        executarExperimentoCompleto(arr_mestre);
        printf("Auto-report concluido. Saindo.\n");
        return EXIT_SUCCESS;
    }

    while (1) {
        limparTela();
        menu();
        
        printf("\nDigite a opcao desejada: ");
        escolha = lerInteiro("");

        if (escolha == 0) {
            printf("\nFinalizando o programa... Obrigado pela atencao!");
            break;
        }

        // Se a opção for válida (1 a 7)
        if (escolha >= 1 && escolha <= 7) {
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
            printf("\nOpcao invalida, por favor insira um numero entre 0 e 6.\n");
            printf("Pressione Enter para continuar...");
            // Limpa o buffer antes de pausar
            while (getchar() != '\n'); 
            getchar(); // Pausa para o usuário ver a mensagem de erro
        }
    }

    return EXIT_SUCCESS;
}


// Determina N0: maior n tal que BubbleSort é mais rapido que MergeSort (usando medias de tempo)
int determinarN0(int arr_mestre[], int maxN) {
    int step = 10; // passo inicial (ajustável)
    int trials = 5; // repetições por tamanho
    int lastBubbleFaster = 0;

    int *copia = malloc(sizeof(int) * TAMANHO);
    if (!copia) return 0;

    int lastPercent = -1;
    for (int n = 2; n <= maxN; n += step) {
        // atualizar indicador de progresso (porcentagem)
        int percent = (int)((100LL * n) / maxN);
        if (percent != lastPercent) {
            printf("\rDeterminando N0: testando n=%d/%d (%d%%)", n, maxN, percent);
            fflush(stdout);
            lastPercent = percent;
        }

        double somaBubble = 0.0;
        double somaMerge = 0.0;

            for (int t = 0; t < trials; t++) {
            // prepara cópias
            copiarArray(arr_mestre, copia, TAMANHO);
                long long troc = 0, comp = 0;
                double tb = bubbleSortTimed(copia, n, &troc, &comp);
                somaBubble += tb;

                copiarArray(arr_mestre, copia, TAMANHO);
                long long mov = 0, compm = 0;
                double tm = mergeSortTimed(copia, 0, n - 1, &mov, &compm);
                somaMerge += tm;
        }

        double avgBubble = somaBubble / trials;
        double avgMerge = somaMerge / trials;

        if (avgBubble <= avgMerge) {
            lastBubbleFaster = n;
        }
    }
    // garante quebra de linha após o progresso
    printf("\n");

    // Refinamento em volta da fronteira encontrada (passo 1)
    int start = lastBubbleFaster > 0 ? lastBubbleFaster : 1;
    int end = start + step;
    if (end > maxN) end = maxN;

    if (end >= start) {
        int total = end - start + 1;
        int lastPct = -1;
        for (int n = start; n <= end; n++) {
            int idx = n - start;
            int pct = (int)((100LL * idx) / total);
            if (pct != lastPct) {
                printf("\rRefinando N0: testando n=%d..%d (%d%%)", start, end, pct);
                fflush(stdout);
                lastPct = pct;
            }

            double somaBubble = 0.0;
            double somaMerge = 0.0;
            for (int t = 0; t < trials; t++) {
                copiarArray(arr_mestre, copia, TAMANHO);
                clock_t ini = clock();
                long long troc = 0, comp = 0;
                bubbleSort(copia, n, &troc, &comp);
                clock_t fim = clock();
                somaBubble += (double)(fim - ini) / CLOCKS_PER_SEC;

                copiarArray(arr_mestre, copia, TAMANHO);
                ini = clock();
                long long mov = 0, compm = 0;
                mergeSort(copia, 0, n - 1, &mov, &compm);
                fim = clock();
                somaMerge += (double)(fim - ini) / CLOCKS_PER_SEC;
            }

            double avgBubble = somaBubble / trials;
            double avgMerge = somaMerge / trials;

            if (avgBubble <= avgMerge) {
                lastBubbleFaster = n;
            }
        }
        printf("\n");
    }

    free(copia);
    return lastBubbleFaster;
}

// Gera um relatorio CSV com tempos amostrados para Bubble e Merge
int gerarRelatorioCSV(int arr_mestre[], int maxN, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    fprintf(f, "n,avgBubble,avgMerge,faster\n");

    int trialsSmall = 5;
    int trialsLarge = 3;

    // Amostragem: 1..200 passo 1, 201..2000 passo 50, >2000 passo 100
    int *copia = malloc(sizeof(int) * TAMANHO);
    if (!copia) { fclose(f); return 0; }

    for (int n = 2; n <= maxN; ) {
        int trials = (n <= 200) ? trialsSmall : trialsLarge;
        double somaBubble = 0.0, somaMerge = 0.0;

        for (int t = 0; t < trials; t++) {
            copiarArray(arr_mestre, copia, TAMANHO);
            long long troc = 0, comp = 0;
            double tb = bubbleSortTimed(copia, n, &troc, &comp);
            somaBubble += tb;

            copiarArray(arr_mestre, copia, TAMANHO);
            long long mov = 0, compm = 0;
            double tm = mergeSortTimed(copia, 0, n - 1, &mov, &compm);
            somaMerge += tm;
        }

        double avgBubble = somaBubble / trials;
        double avgMerge = somaMerge / trials;
        const char *faster = (avgBubble <= avgMerge) ? "Bubble" : "Merge";
        fprintf(f, "%d,%.9f,%.9f,%s\n", n, avgBubble, avgMerge, faster);

        // Parada dinâmica desativada (execução seguirá até maxN conforme solicitação)

        // Próximo n com regras de amostragem
        if (n < 200) n += 1;
        else if (n < 2000) n += 50;
        else n += 100;
    }

    free(copia);
    fclose(f);
    return 1;
}

// Executa experimento completo: para coleção ordenada e inversa
void executarExperimentoCompleto(int arr_mestre[TAMANHO]) {
    int copia[TAMANHO];
    double times_bubble[REPETICOES];
    double times_merge[REPETICOES];
    double times_hibrido[REPETICOES];

    // Colecao ordenada
    printf("\n== Experimento: COLECAO ORDENADA ==\n");
    gerarArrayOrdenado(arr_mestre, TAMANHO);
    // Determina N0 usando rotina existente
    printf("Determinando N0 para colecao ordenada...\n");
    int n0 = determinarN0(arr_mestre, TAMANHO);
    printf("N0 (ordenada) = %d\n", n0);

    for (int i = 0; i < REPETICOES; i++) {
        // Bubble (timed wrapper)
        copiarArray(arr_mestre, copia, TAMANHO);
        long long tb_trocas = 0, tb_comps = 0;
        double t_b = bubbleSortTimed(copia, TAMANHO, &tb_trocas, &tb_comps);
        times_bubble[i] = t_b;

        // Merge (timed wrapper)
        copiarArray(arr_mestre, copia, TAMANHO);
        long long m_mov = 0, m_comp = 0;
        double t_m = mergeSortTimed(copia, 0, TAMANHO - 1, &m_mov, &m_comp);
        times_merge[i] = t_m;

        // Hibrido (timed wrapper)
        copiarArray(arr_mestre, copia, TAMANHO);
        long long h_mov = 0, h_comp = 0;
        double t_h = mergeSortHibridoTimed(copia, 0, TAMANHO - 1, n0, &h_mov, &h_comp);
        times_hibrido[i] = t_h;

        if ((i + 1) % 10 == 0) printf("Ordenada: repeticoes %d/%d\n", i+1, REPETICOES);
    }

    // Salvar CSV com resultados brutos
    salvarResultadosCSV("resultados_ordenado.csv", times_bubble, times_merge, times_hibrido, REPETICOES, n0, 1);

    // Estatísticas resumidas
    double min, max, media, moda, desvio;
    printf("\nResumo (Ordenada):\n");
    printf("-- Bubble --\n");
    calcularEstatisticas(times_bubble, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("-- Merge --\n");
    calcularEstatisticas(times_merge, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("-- Hibrido (n0=%d) --\n", n0);
    calcularEstatisticas(times_hibrido, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    // Agora a colecao inversa
    printf("\n== Experimento: COLECAO INVERSA ==\n");
    gerarArrayInverso(arr_mestre, TAMANHO);
    printf("Determinando N0 para colecao inversa...\n");
    n0 = determinarN0(arr_mestre, TAMANHO);
    printf("N0 (inversa) = %d\n", n0);

    for (int i = 0; i < REPETICOES; i++) {
        // Bubble
        copiarArray(arr_mestre, copia, TAMANHO);
        long long tb_trocas = 0, tb_comps = 0;
        double t_b = bubbleSortTimed(copia, TAMANHO, &tb_trocas, &tb_comps);
        times_bubble[i] = t_b;

        // Merge
        copiarArray(arr_mestre, copia, TAMANHO);
        long long m_mov = 0, m_comp = 0;
        double t_m = mergeSortTimed(copia, 0, TAMANHO - 1, &m_mov, &m_comp);
        times_merge[i] = t_m;

        // Hibrido
        copiarArray(arr_mestre, copia, TAMANHO);
        long long h_mov = 0, h_comp = 0;
        double t_h = mergeSortHibridoTimed(copia, 0, TAMANHO - 1, n0, &h_mov, &h_comp);
        times_hibrido[i] = t_h;

        if ((i + 1) % 10 == 0) printf("Inversa: repeticoes %d/%d\n", i+1, REPETICOES);
    }

    salvarResultadosCSV("resultados_inverso.csv", times_bubble, times_merge, times_hibrido, REPETICOES, n0, 2);

    // Estatísticas resumidas (Inversa)
    printf("\nResumo (Inversa):\n");
    printf("-- Bubble --\n");
    calcularEstatisticas(times_bubble, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("-- Merge --\n");
    calcularEstatisticas(times_merge, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("-- Hibrido (n0=%d) --\n", n0);
    calcularEstatisticas(times_hibrido, REPETICOES, &min, &max, &media, &moda, &desvio);
    printf("Min: %.9f  Max: %.9f  Media: %.9f  Moda: %.9f  Desvio: %.9f\n", min, max, media, moda, desvio);

    printf("\nExperimento completo (ordenado + inverso) concluido. CSVs gerados.\n");
}