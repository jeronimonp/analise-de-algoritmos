# 📊 Análise de Performance de Algoritmos de Ordenação (C)

Aqui conterá o código-fonte em C desenvolvido para o projeto de Análise de Algoritmos. O objetivo principal é comparar o desempenho dos algoritmos Quadrático (**Bubble Sort**), Log-Linear (**Merge Sort**) e um **Algoritmo Híbrido** em diferentes cenários de dados (ordenados e inversos).

---

## 🚀 Como Compilar e Rodar o Programa

### 1. Requisitos

Você precisará do compilador **GCC** instalado no seu sistema (Linux, macOS ou Windows/WSL).

### 2. Estrutura do Projeto

O código foi dividido em três arquivos para melhor organização:

| Arquivo | Descrição |
| :--- | :--- |
| `main.c` | Contém o Menu principal e a lógica de controle. |
| `sort.c` | Implementa os algoritmos de ordenação (Bubble, Merge e Híbrido). |
| `utils.c` | Funções de suporte (leitura de input, limpeza de tela, geração de arrays). |
| `sort.h` | Protótipos das funções e constantes (`TAMANHO`). |

### 3. Compilação

Para compilar o projeto, abra seu terminal (Bash, PowerShell, CMD) no diretório raiz do projeto e use o comando:

```bash
gcc main.c sort.c utils.c -o main

## Lembre-se de usar './' para rodar o executável no diretório atual
./meu_algoritmo