#!/usr/bin/env python3
import sys
import csv
import argparse
import matplotlib.pyplot as plt


def plot(csv_file, out_png='report.png', logy=False, mark_n0=True):
    ns = []
    bubble = []
    merge = []
    faster = []
    with open(csv_file, newline='') as f:
        reader = csv.DictReader(f)
        for row in reader:
            ns.append(int(row['n']))
            bubble.append(float(row['avgBubble']))
            merge.append(float(row['avgMerge']))
            faster.append(row.get('faster', ''))

    # compute N0: largest n where 'faster' is Bubble
    n0 = None
    for n, f in zip(ns, faster):
        if f.strip().lower() == 'bubble':
            n0 = n

    plt.figure(figsize=(10,6))
    plt.plot(ns, bubble, label='Bubble', marker='o', markersize=4)
    plt.plot(ns, merge, label='Merge', marker='x', markersize=4)

    if mark_n0 and n0 is not None:
        # find y position for annotation (use max of the two at n0)
        try:
            idx = ns.index(n0)
            yval = max(bubble[idx], merge[idx])
            plt.axvline(x=n0, color='gray', linestyle='--', linewidth=1)
            plt.annotate(f'N0 = {n0}', xy=(n0, yval), xytext=(n0, yval*1.5 if yval>0 else yval+1e-6),
                         arrowprops=dict(arrowstyle='->', color='black'))
        except ValueError:
            pass

    plt.xlabel('n')
    plt.ylabel('Tempo (s)')
    plt.title('Comparacao: Bubble vs Merge')
    plt.legend()
    plt.grid(True)

    if logy:
        plt.yscale('log')

    plt.tight_layout()
    plt.savefig(out_png)
    print(f'Grafico salvo em {out_png} (logy={logy}, mark_n0={mark_n0})')


def parse_args():
    p = argparse.ArgumentParser(description='Plota report CSV comparando Bubble vs Merge')
    p.add_argument('csv', help='Arquivo CSV gerado pelo programa')
    p.add_argument('out', nargs='?', default='report.png', help='Arquivo de saída PNG')
    p.add_argument('--logy', action='store_true', help='Usar escala logarítmica no eixo Y')
    p.add_argument('--no-n0', dest='mark_n0', action='store_false', help='Não marcar N0 no gráfico')
    return p.parse_args()


if __name__ == '__main__':
    args = parse_args()
    plot(args.csv, args.out, logy=args.logy, mark_n0=args.mark_n0)
