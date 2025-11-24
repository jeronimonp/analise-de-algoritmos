#!/usr/bin/env python3
"""
Versão 'bonita' do plot_results_three.py
- Boxplot com pontos (jitter), paleta agradável
- Gráfico de médias com erro (std)
- Marca N0 (se encontrado nos comentários do CSV)
- Salva PNGs em alta resolução

Uso:
    python3 plot_results_three_pretty.py resultados_inverso.csv --out inversa_pretty

Requer: matplotlib, numpy (opcional: seaborn para estilo melhor)
"""
import argparse
import csv
import sys
import re
import math

import numpy as np
import matplotlib.pyplot as plt

# tenta importar seaborn para estilos mais bonitos, mas não é obrigatório
try:
    import seaborn as sns
    seaborn_available = True
except Exception:
    seaborn_available = False


def read_three_csv(path):
    bubble = []
    merge = []
    hibrido = []
    meta_lines = []
    with open(path, newline='') as f:
        for line in f:
            if line.startswith('#'):
                meta_lines.append(line.strip())
            else:
                # first non-meta line is header; remaining are data
                break
    # now use csv reader skipping lines that start with '#'
    with open(path, newline='') as f:
        reader = csv.DictReader(l for l in f if not l.startswith('#'))
        for row in reader:
            try:
                bubble.append(float(row['bubble']))
                merge.append(float(row['merge']))
                hibrido.append(float(row['hibrido']))
            except Exception:
                continue
    return np.array(bubble), np.array(merge), np.array(hibrido), meta_lines


def find_n0(meta_lines):
    # procura por algo como 'N0=52' ou 'N0 (inversa) = 52' nas linhas de metadados
    for ln in meta_lines:
        m = re.search(r'N0\s*=?\s*(\d+)', ln)
        if m:
            return int(m.group(1))
    return None


def jitter(arr, amount=0.03):
    return (np.random.rand(arr.size) - 0.5) * amount


def plot_boxplot_with_points(bubble, merge, hibrido, out_png, logy=False, title=None):
    labels = ['Bubble', 'Merge', 'Hibrido']
    data = [bubble, merge, hibrido]

    plt.figure(figsize=(9,6), dpi=180)
    if seaborn_available:
        sns.set_style('whitegrid')
        palette = sns.color_palette('Set2', n_colors=3)
    else:
        plt.style.use('seaborn-whitegrid')
        palette = ['#a6cee3', '#1f78b4', '#b2df8a']

    # boxplot
    bplot = plt.boxplot(data, labels=labels, patch_artist=True, showmeans=True, widths=0.6)
    for patch, color in zip(bplot['boxes'], palette):
        patch.set_facecolor(color)
        patch.set_alpha(0.9)

    # pontos com jitter (sobreposição leve)
    for i, arr in enumerate(data, start=1):
        x = np.full(arr.shape, i) + jitter(arr, amount=0.08)
        plt.scatter(x, arr, color='gray', alpha=0.6, s=18, edgecolors='none')

    plt.ylabel('Tempo (s)')
    if title:
        plt.title(title)
    else:
        plt.title('Boxplot: tempos por algoritmo (100 execuções)')

    if logy:
        plt.yscale('log')

    plt.grid(axis='y', linestyle='--', alpha=0.5)
    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Salvo boxplot em: {out_png}')
    plt.close()


def plot_means(bubble, merge, hibrido, out_png, logy=False, annotate_n0=None):
    labels = ['Bubble', 'Merge', 'Hibrido']
    means = [np.mean(bubble), np.mean(merge), np.mean(hibrido)]
    stds = [np.std(bubble, ddof=0), np.std(merge, ddof=0), np.std(hibrido, ddof=0)]

    plt.figure(figsize=(8,5), dpi=180)
    x = np.arange(len(labels))
    if seaborn_available:
        palette = sns.color_palette('Set2', n_colors=3)
    else:
        palette = ['#1f77b4', '#ff7f0e', '#2ca02c']

    bars = plt.bar(x, means, yerr=stds, capsize=8, color=palette, alpha=0.9)
    plt.xticks(x, labels)
    plt.ylabel('Tempo médio (s)')
    plt.title('Média e Desvio Padrão (100 execuções)')

    # anotar valores acima
    for xi, m, s in zip(x, means, stds):
        plt.text(xi, m + max(1e-9, s) * 0.6, f'{m:.6f}s', ha='center', va='bottom', fontsize=9)

    if annotate_n0 is not None:
        # desenha linha vertical aproximada no gráfico de médias indicando N0 (apenas como label)
        plt.annotate(f'N0 = {annotate_n0}', xy=(0.95, 0.95), xycoords='axes fraction',
                     ha='right', va='top', fontsize=9, bbox=dict(boxstyle='round', fc='wheat', alpha=0.6))

    if logy:
        plt.yscale('log')

    plt.grid(axis='y', linestyle='--', alpha=0.4)
    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Salvo gráfico de médias em: {out_png}')
    plt.close()


def plot_timeseries(bubble, merge, hibrido, out_png, logy=False, title=None):
    plt.figure(figsize=(10,5), dpi=180)
    if seaborn_available:
        sns.set_style('whitegrid')
        palette = sns.color_palette('Set2', n_colors=3)
    else:
        plt.style.use('seaborn-whitegrid')
        palette = ['#1f77b4', '#ff7f0e', '#2ca02c']

    runs = np.arange(1, bubble.size + 1)
    plt.plot(runs, bubble, label='Bubble', color=palette[0], marker='o', markersize=4, linewidth=1)
    plt.plot(runs, merge, label='Merge', color=palette[1], marker='s', markersize=4, linewidth=1)
    plt.plot(runs, hibrido, label='Hibrido', color=palette[2], marker='^', markersize=4, linewidth=1)

    plt.xlabel('Execução (índice)')
    plt.ylabel('Tempo (s)')
    if title:
        plt.title(title)
    else:
        plt.title('Tempo por execução (100 runs)')
    plt.legend()
    plt.grid(alpha=0.4)

    # anotar médias na legenda/plot
    mb = np.mean(bubble)
    mm = np.mean(merge)
    mh = np.mean(hibrido)
    plt.annotate(f'Bubble mean: {mb:.6f}s', xy=(0.99, 0.95), xycoords='axes fraction', ha='right')
    plt.annotate(f'Merge mean: {mm:.6f}s', xy=(0.99, 0.90), xycoords='axes fraction', ha='right')
    plt.annotate(f'Hibrido mean: {mh:.6f}s', xy=(0.99, 0.85), xycoords='axes fraction', ha='right')

    if logy:
        plt.yscale('log')

    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Salvo timeseries em: {out_png}')
    plt.close()


def main():
    p = argparse.ArgumentParser(description='Plotagem aprimorada para resultados (3 algoritmos)')
    p.add_argument('csv', help='Arquivo CSV gerado pelo programa (resultados_*.csv)')
    p.add_argument('--out', default='report_pretty', help='Prefixo do arquivo de saída')
    p.add_argument('--logy', action='store_true', help='Usar escala logarítmica no eixo Y')
    p.add_argument('--timeseries', action='store_true', help='Gerar gráfico de série temporal (tempo por execução)')
    p.add_argument('--normalize', action='store_true', help='Normalizar tempos dividindo tudo pela média do Bubble (torna curvas comparáveis)')
    p.add_argument('--ratio', action='store_true', help='Gerar gráfico de razões Merge/Bubble e Hibrido/Bubble')
    p.add_argument('--inset', action='store_true', help='Adicionar um inset (zoom) no timeseries/boxplot para valores pequenos')
    args = p.parse_args()

    try:
        bubble, merge, hibrido, meta = read_three_csv(args.csv)
    except Exception as e:
        print('Erro ao ler CSV:', e)
        sys.exit(1)

    n0 = find_n0(meta)

    # aplica normalizacao se pedido
    if args.normalize:
        ref_mean = np.mean(bubble)
        if ref_mean > 0:
            bubble = bubble / ref_mean
            merge = merge / ref_mean
            hibrido = hibrido / ref_mean

    # imprime resumo no terminal (mais compacto)
    def s(arr):
        return (float(np.min(arr)), float(np.max(arr)), float(np.mean(arr)), float(np.std(arr, ddof=0)))
    bmin, bmax, bmean, bstd = s(bubble)
    mmin, mmax, mmean, mstd = s(merge)
    hmin, hmax, hmean, hstd = s(hibrido)

    print('\nResumo estatístico (arquivo: {})'.format(args.csv))
    print('-- Bubble --')
    print(f'Min: {bmin:.9f}  Max: {bmax:.9f}  Mean: {bmean:.9f}  Std: {bstd:.9f}')
    print('-- Merge --')
    print(f'Min: {mmin:.9f}  Max: {mmax:.9f}  Mean: {mmean:.9f}  Std: {mstd:.9f}')
    print('-- Hibrido --')
    print(f'Min: {hmin:.9f}  Max: {hmax:.9f}  Mean: {hmean:.9f}  Std: {hstd:.9f}')
    if n0 is not None:
        print(f'Encontrado N0 nos metadados: {n0}')

    title = f'Resultados: {args.csv}'
    box_png = args.out + '_boxplot.png'
    means_png = args.out + '_means.png'
    if args.normalize:
        box_png = args.out + '_boxplot_normalized.png'
        means_png = args.out + '_means_normalized.png'

    plot_boxplot_with_points(bubble, merge, hibrido, box_png, logy=args.logy, title=title)
    plot_means(bubble, merge, hibrido, means_png, logy=args.logy, annotate_n0=n0)
    if args.timeseries:
        times_png = args.out + '_timeseries.png'
        plot_timeseries(bubble, merge, hibrido, times_png, logy=args.logy, title=title)
    if args.ratio:
        # plot ratios as timeseries and bar of mean ratios
        # avoid division by zero by adding small epsilon
        eps = 1e-12
        ratio_m = merge / (bubble + eps)
        ratio_h = hibrido / (bubble + eps)
        ratio_ts = args.out + '_ratio_timeseries.png'
        plt.figure(figsize=(10,4), dpi=150)
        runs = np.arange(1, ratio_m.size + 1)
        plt.plot(runs, ratio_m, label='Merge/Bubble', color='#ff7f0e', linewidth=1)
        plt.plot(runs, ratio_h, label='Hibrido/Bubble', color='#2ca02c', linewidth=1)
        plt.xlabel('Execução')
        plt.ylabel('Razão (algoritmo / Bubble)')
        plt.title('Razões por execução')
        plt.legend()
        plt.grid(alpha=0.4)
        if args.logy:
            plt.yscale('log')
        plt.tight_layout()
        plt.savefig(ratio_ts)
        plt.close()
        print(f'Salvo ratio timeseries em: {ratio_ts}')

        # barras de médias de razão
        mean_rm = np.mean(ratio_m)
        mean_rh = np.mean(ratio_h)
        bars_png = args.out + '_ratio_means.png'
        plt.figure(figsize=(6,4), dpi=150)
        plt.bar([0,1], [mean_rm, mean_rh], color=['#ff7f0e', '#2ca02c'])
        plt.xticks([0,1], ['Merge/Bubble','Hibrido/Bubble'])
        plt.ylabel('Média da razão')
        plt.title('Média das razões (Merge/Bubble, Hibrido/Bubble)')
        plt.grid(axis='y', alpha=0.4)
        plt.tight_layout()
        plt.savefig(bars_png)
        plt.close()
        print(f'Salvo ratio means em: {bars_png}')

    print('\nArquivos gerados:')
    print(' -', box_png)
    print(' -', means_png)

if __name__ == '__main__':
    main()
