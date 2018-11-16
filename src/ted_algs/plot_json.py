#!/usr/bin/env python3

# The MIT License (MIT)
# Copyright (c) 2017 Mateusz Pawlik.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Solving computing plots on the server. Taken from:
# https://stackoverflow.com/questions/37604289/tkinter-tclerror-no-display-name-and-no-display-environment-variable
import os
import matplotlib
if os.environ.get('DISPLAY','') == '':
    print('No display found. Using non-interactive Agg (Anti-grain geometry engine) backend.')
    matplotlib.use('Agg')
# End: Solving computing plots on the server

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
    'LabelGuided' : 'lg',
    'LabelGuidedDeprecated' : 'lgdepr'
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
    '--pick',
    dest = 'pick',
    type = int,
    default = 0,
    help = 'Pick every pick\'th datapoint.'
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
    '--print-num-inf',
    dest = 'print_num_inf',
    action = 'store_true',
    help = 'Set if count of inf values should be printed (default: not set).'
)
parser.add_argument(
    '--print-num-zero',
    dest = 'print_num_zero',
    action = 'store_true',
    help = 'Set if count of zero values should be printed (default: not set).'
)
parser.add_argument(
    '--logscale',
    dest = 'logscale',
    action = 'store_true',
    help = 'Set legscale for Y-axis (default: not set).'
)
parser.add_argument(
    '--no-lines',
    dest = 'no_lines',
    action = 'store_true',
    help = 'Remove the lines form the plot (default: not set).'
)
parser.add_argument(
    '--avg-per-x-value',
    dest = 'avg_per_x_value',
    action = 'store_true',
    help = 'Compute averages if multiple points per x-value.'
)
parser.add_argument(
    '--show',
    dest = 'show',
    action = 'store_true',
    help = 'Display the plot after processing.'
)
args = parser.parse_args()

with open(args.input_filename, "r") as f:
    data = json.load(f)

fig, ax = plt.subplots()

lines = ['-','--','-.',':']
if args.no_lines:
    lines = [' ']
linecycler = cycle(lines)
markers = ['x','+','o','.']
markercycler = cycle(markers)

df = json_normalize(data['algorithm_executions'], ['data_items'], meta=['algorithm_name'])
df = df.set_index(args.xlabel)
groups = df.groupby('algorithm_name')[args.measure]

line_labels = []
for name, group in groups:
    if (args.avg_per_x_value):
        group = group.groupby(level=0).mean() # Not really sure about this line.
    if (algorithms[name] in args.algs):
        if (args.pick > 0):
            group = group[0::args.pick]
        ax.plot(group, label=name, linestyle=next(linecycler), marker=next(markercycler))
        line_labels.append(name)

ax.set(xlabel=xlabels[args.xlabel], ylabel=measures[args.measure], title='TED algorithms ('+os.path.split(args.input_filename)[1]+')')
ax.grid()

if args.logscale:
    ax.set_yscale('log')

if args.print_num_inf:
    dcsummary = pandas.DataFrame([df.groupby('algorithm_name')[args.measure].apply(lambda x: x[x == float('inf')].count())],index=['inf count'])
    dcsummary = dcsummary.transpose()
    ax.legend([x[0] + ': ' + str(x[1]) for x in zip(line_labels,dcsummary['inf count'].tolist())])
elif args.print_num_zero:
    dcsummary = pandas.DataFrame([df.groupby('algorithm_name')[args.measure].apply(lambda x: x[x == 0].count())],index=['inf count'])
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
fig.savefig(os.path.split(args.input_filename)[1] + '_' + args.measure +
    ('_logY' if args.logscale else '') +
    (('_pick_' + str(args.pick)) if args.pick > 0 else '') +
    '.pdf', bbox_inches='tight')

if args.show:
    plt.show()