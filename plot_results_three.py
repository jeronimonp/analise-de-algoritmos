#!/usr/bin/env python3
"""
Plota boxplot e médias para CSVs gerados pelo programa:
Arquivo esperado (padrão de `salvarResultadosCSV`):
- Linhas iniciais com metadados iniciando com '#'
- Header: idx,bubble,merge,hibrido
- 100 linhas com tempos em segundos

Uso:
    python plot_results_three.py resultados_inverso.csv --out out_inversa --logy

Gera:
 - out_inversa_boxplot.png  (boxplot das 3 abordagens)
 - out_inversa_means.png    (barra das médias com desvio padrão)

Requer: matplotlib, numpy
"""
import argparse
import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
from statistics import mean, pstdev


def read_three_csv(path):
    bubble = []
    merge = []
    hibrido = []
    meta = []
    with open(path, newline='') as f:
        # skip commented metadata lines
        lines = [l for l in f if not l.startswith('#')]
    if not lines:
        raise ValueError('Arquivo vazio ou formato inválido')
    reader = csv.DictReader(lines)
    for row in reader:
        try:
            bubble.append(float(row['bubble']))
            merge.append(float(row['merge']))
            hibrido.append(float(row['hibrido']))
        except Exception as e:
            # skip malformed rows
            continue
    return np.array(bubble), np.array(merge), np.array(hibrido)


def summarize(arr, name):
    arr_clean = arr[~np.isnan(arr)]
    if arr_clean.size == 0:
        return {'min': np.nan, 'max': np.nan, 'mean': np.nan, 'std': np.nan}
    return {'min': float(np.min(arr_clean)), 'max': float(np.max(arr_clean)),
            'mean': float(np.mean(arr_clean)), 'std': float(np.std(arr_clean, ddof=0))}


def plot_boxplot(bubble, merge, hibrido, out_png, logy=False):
    plt.figure(figsize=(8,6))
    data = [bubble, merge, hibrido]
    labels = ['Bubble', 'Merge', 'Hibrido']
    plt.boxplot(data, labels=labels, showmeans=True)
    plt.ylabel('Tempo (s)')
    plt.title('Boxplot: tempos por algoritmo (100 execuções)')
    if logy:
        plt.yscale('log')
    plt.grid(axis='y', linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Salvo boxplot em: {out_png}')


def plot_means(bubble, merge, hibrido, out_png, logy=False):
    means = [np.mean(bubble), np.mean(merge), np.mean(hibrido)]
    stds = [np.std(bubble, ddof=0), np.std(merge, ddof=0), np.std(hibrido, ddof=0)]
    labels = ['Bubble', 'Merge', 'Hibrido']

    plt.figure(figsize=(8,5))
    x = np.arange(len(labels))
    plt.bar(x, means, yerr=stds, capsize=6)
    plt.xticks(x, labels)
    plt.ylabel('Tempo médio (s)')
    plt.title('Média e Desvio Padrão (100 execuções)')
    if logy:
        plt.yscale('log')
    plt.grid(axis='y', linestyle='--', alpha=0.6)
    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Salvo gráfico de médias em: {out_png}')


def main():
    p = argparse.ArgumentParser(description='Plota resultados (3 algoritmos) salvos pelo programa C')
    p.add_argument('csv', help='Arquivo CSV gerado (resultados_*.csv)')
    p.add_argument('--out', default='report_three', help='Prefixo do arquivo de saída (sem extensão)')
    p.add_argument('--logy', action='store_true', help='Usar escala logarítmica no eixo Y')
    args = p.parse_args()

    try:
        bubble, merge, hibrido = read_three_csv(args.csv)
    except Exception as e:
        print('Erro ao ler CSV:', e)
        sys.exit(1)

    # imprimir resumo no terminal
    sb = summarize(bubble, 'Bubble')
    sm = summarize(merge, 'Merge')
    sh = summarize(hibrido, 'Hibrido')

    print('\nResumo estatístico (arquivo: {})'.format(args.csv))
    print('-- Bubble --')
    print('Min: {min:.9f}  Max: {max:.9f}  Mean: {mean:.9f}  Std: {std:.9f}'.format(**sb))
    print('-- Merge --')
    print('Min: {min:.9f}  Max: {max:.9f}  Mean: {mean:.9f}  Std: {std:.9f}'.format(**sm))
    print('-- Hibrido --')
    print('Min: {min:.9f}  Max: {max:.9f}  Mean: {mean:.9f}  Std: {std:.9f}'.format(**sh))

    # gerar graficos
    box_png = args.out + '_boxplot.png'
    means_png = args.out + '_means.png'
    plot_boxplot(bubble, merge, hibrido, box_png, logy=args.logy)
    plot_means(bubble, merge, hibrido, means_png, logy=args.logy)


if __name__ == '__main__':
    main()
