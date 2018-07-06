#!/usr/bin/python3
import argparse
import os
import json 
import pandas
import numpy
import math
import matplotlib.pyplot as plt
from pandas.io.json import json_normalize
from matplotlib.ticker import MaxNLocator
from itertools import cycle
from matplotlib.lines import Line2D


measures = {
    'runtime'       : 'runtime (ms)',
    'subproblems'   : '#subproblems',
    'ted'           : 'TED value',
    'tree_size_1'   : 'tree size (#nodes)',
    'top_y_updates' : 'top_y node updates (#times)'
}

xlabels = {
    'tree_id_1'   : 'tree id in collection',
    'tree_size_1' : 'size of left-hand tree (#nodes)',
    'k'           : 'similarity threshold'
}

algorithms = {
    'ZhangShasha' : 'zs',
    'APTED' : 'apted',
    'Touzet' : 'tz',
    'TouzetDP' : 'tzd',
    'TouzetKrSet' : 'tzs',
    'TouzetKrLoop' : 'tzl',
    'TouzetKrSetEmax' : 'tzse',
    'TouzetKrLoopEmax' : 'tzle',
    'LabelGuided' : 'lg'
}

parser = argparse.ArgumentParser()
parser.add_argument(
    type = str,
    dest = 'input_filename',
    help = "Path to experiment results json file."
)
parser.add_argument(
    '--measure',
    dest = 'measure',
    type = str,
    default = 'runtime',
    choices = measures.keys(),
    help = 'Choose measure to plot (default: runtime).'
)
parser.add_argument(
    '--xlabel',
    dest = 'xlabel',
    type = str,
    default = 'tree_id_1',
    choices = xlabels.keys(),
    help = 'Choose labels on X axis to plot (default: tree_id_1). NOTE:\
            Choosing tree_size_1 requires input dataset to be sorted by tree\
            size.'
)
parser.add_argument(
    '--algs',
    dest = 'algs',
    nargs = '+',
    choices = algorithms.values(),
    default = algorithms.values(),
    help = 'Choose the algorithms to plot (default: all).'
)
parser.add_argument(
    '--print-inf',
    dest = 'print_inf',
    action = 'store_true',
    help = 'Set if inf values should be printed as some high value (default: not set).'
)
args = parser.parse_args()

with open(args.input_filename, "r") as f:
    data = json.load(f)

fig, ax = plt.subplots()

lines = ['-','--','-.',':']
linecycler = cycle(lines)
markers = ['x','+','o','.']
markercycler = cycle(markers)

df = json_normalize(data['algorithm_executions'], ['data_items'], meta=['algorithm_name'])
df = df.set_index(args.xlabel)
groups = df.groupby('algorithm_name')[args.measure]

line_labels = []
for name, group in groups:
    if (algorithms[name] in args.algs):
        ax.plot(group, label=name, linestyle=next(linecycler), marker=next(markercycler))
        line_labels.append(name)

ax.set(xlabel=xlabels[args.xlabel], ylabel=measures[args.measure], title='TED algorithms ('+os.path.split(args.input_filename)[1]+')')
ax.grid()

if args.print_inf:
    dcsummary = pandas.DataFrame([df.groupby('algorithm_name')[args.measure].apply(lambda x: x[x == float('inf')].count())],index=['inf count'])
    dcsummary = dcsummary.transpose()
    ax.legend([x[0] + ': ' + str(x[1]) for x in zip(line_labels,dcsummary['inf count'].tolist())])
else:
    ax.legend()

ax.xaxis.set_major_locator(MaxNLocator(integer=True))

# TABLE CONSTRUCTION LEFT FOR REFERENCE
# dcsummary = pandas.DataFrame([df.groupby('algorithm_name')[args.measure].apply(lambda x: x[x == float('inf')].count())],index=['inf count'])
# dcsummary = dcsummary.transpose()
# colors = ['b' for _ in dcsummary.values]
# the_table = plt.table(cellText=dcsummary.values,
#           rowLabels=dcsummary.index,
#           colLabels=dcsummary.columns,
#           colWidths=[0.1] * len(dcsummary.columns),
#           cellLoc = 'center', rowLoc = 'center',
#           loc='best',
#           # bbox=(0.2,0.05,0.1*len(dcsummary.columns),0.1*len(dcsummary.index)),
#           # cellColours = colors,
#           alpha=0.1,
#           zorder=2
#           )
# the_table.set_alpha(0.1)
# # the_table.auto_set_font_size(False)
# # the_table.set_fontsize(9)
# # the_table.scale(2, 2)
# 
# plt.legend(dcsummary)

fig.set_size_inches(8,6)
fig.savefig(os.path.split(args.input_filename)[1]+'.pdf', bbox_inches='tight')

plt.show()