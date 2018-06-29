import argparse
import os
import json 
import pandas
import numpy
import matplotlib.pyplot as plt
from pandas.io.json import json_normalize
from matplotlib.ticker import MaxNLocator

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
    choices=['runtime', 'subproblems', 'ted'],
    help='Choose measure to plot (default: runtime)'
)
args = parser.parse_args()

measures = {
    'runtime' : 'runtime (ms)',
    'subproblems' : '#subproblems',
    'ted' : 'TED value'
}

with open(args.input_filename, "r") as f:
    data = json.load(f)

fig, ax = plt.subplots()

df = json_normalize(data['algorithm_executions'], ['data_items'], meta=['algorithm_name'])
# df.sort_values(by=['tree_size_1'], inplace=True)
df.set_index('tree_id_1', inplace=True)
df.groupby('algorithm_name')[args.measure].plot(ax=ax)

ax.set(xlabel='tree id', ylabel=measures[args.measure], title='TED algorithms ('+os.path.split(args.input_filename)[1]+')')
ax.grid()
ax.legend()
ax.xaxis.set_major_locator(MaxNLocator(integer=True))

fig.savefig(os.path.split(args.input_filename)[1]+'.pdf', bbox_inches='tight')

plt.show()