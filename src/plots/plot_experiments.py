#!/usr/bin/env python
'''
    File name: plot_experiments.py
    Author: Thomas Huetter
    Program: Outputs a plot specified in a given config file.
             The data is requested from the ted-join database.
'''

import argparse
import json
import os
import numpy as np
import matplotlib.pyplot as plt
import psycopg2
from psycopg2 import sql
from matplotlib.ticker import EngFormatter

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def db_request(select_args, table_args, where_args, group_args, order_args):
  # Connect to database.
  db = psycopg2.connect("service=ted-join")
  # # Open a cursor to perform database operations
  cur = db.cursor()
  # Build query
  query = "SELECT " + select_args + " FROM " + table_args + where_args + group_args + order_args
  print(query)
  cur.execute(query)
  # Fetch query result
  rows = cur.fetchall()
  print("\nShow me the databases:\n")
  for row in rows:
      print("   ", row)
  db.commit()
  # # Close the cursor.
  cur.close()
  # # Close communication with the database.
  db.close()

  return rows


# parse input argurments
parser = argparse.ArgumentParser()
parser.add_argument("--config", type=str, 
  help="path to input config file (json")
parser.add_argument("--storeplot", type=str, default="", 
  help="path and filename where the plot should be stored")
args = parser.parse_args()

path, filename = os.path.split(args.config)

with open(args.config) as f:
    data = json.load(f)

fig, ax = plt.subplots()

# set title
if data['print_title'] != 'no' and 'title' in data:
  ax.title(data['title'] + " - " + data['dataset_name'])
# set grid
if 'grid' in data:
  ax.grid(data['grid'])

marker_cnt = 0
y = []
result = {}
plot_parameters = {}
# execute the query
for table in data['tables']:
  for attr in table['attributes']:
    print(" === TABLE NAME === ")
    if 'aggregate' in data['y_axis']:
      select_args = data['x_axis']['db_column'] + ", " + data['y_axis']['aggregate'] + "(" + attr['attr_name'] + ")"
    else:
      select_args = data['x_axis']['db_column'] + ", " + attr['attr_name']
    if 'factor' in attr:
      select_args += attr['factor']
    table_args = table['table_name']
    where_args = ''
    if 'constraints' in data:
      where_args = ' WHERE '
      where_args += ' AND '.join("{!s}={!r}".format(key,val) for (key,val) in data['constraints'].items())
    if 'constraints' in table:
      where_args += ' AND '
      where_args += ' AND '.join("{!s}={!r}".format(key,val) for (key,val) in table['constraints'].items())
    if 'db_column' in data['x_axis']:
      group_args = ' GROUP BY ' + data['x_axis']['db_column']
      order_args = ' ORDER BY ' + data['x_axis']['db_column']
    result = db_request(select_args, table_args, where_args, group_args, order_args)

    label = ""
    if 'name' in table:
      label += table['name']
    if 'name' in attr:
      label += attr['name']
    if label != '':
      plot_parameters['label'] = label
    if 'markers' in data:
      plot_parameters['marker'] = data['markers'][marker_cnt]
    if 'markersize' in data:
      plot_parameters['markersize'] = data['markersize'][marker_cnt]
    if 'markerfacecolor' in data:
      plot_parameters['mfc'] = data['markerfacecolor']
    if 'colors' in data:
      plot_parameters['color'] = data['colors'][marker_cnt]
    if 'lines' in data:
      plot_parameters['linestyle'] = data['lines'][marker_cnt]

    ax.plot(*zip(*result), **plot_parameters)
    marker_cnt += 1

# set axis range
x_axis_font_size = 14
if 'font_size' in data['x_axis']:
  x_axis_font_size = data['x_axis']['font_size']
y_axis_font_size = 14
if 'font_size' in data['y_axis']:
  y_axis_font_size = data['y_axis']['font_size']
plt.xlabel(data['x_axis']['name'], fontsize=x_axis_font_size)
plt.ylabel(data['y_axis']['name'], fontsize=y_axis_font_size)
if 'ticks_font_size' in data['x_axis']:
  plt.xticks(fontsize=data['x_axis']['ticks_font_size'])
if 'ticks_font_size' in data['y_axis']:
  plt.yticks(fontsize=data['y_axis']['ticks_font_size'])
if 'scale' in data['y_axis']:
  ax.set_yscale(data['y_axis']['scale'], nonposy='clip')
if 'ymin' in data['y_axis']:
  ax.set_ylim(ymin=data['y_axis']['ymin'])
if 'ymax' in data['y_axis']:
  ax.set_ylim(ymax=data['y_axis']['ymax'])
if 'xmin' in data['x_axis']:
  ax.set_xlim(xmin=data['x_axis']['xmin'])
if 'xmax' in data['x_axis']:
  ax.set_xlim(xmax=data['x_axis']['xmax'])
if 'xticks' in data['x_axis']:
  x1,x2,y1,y2 = plt.axis()
  plt.xticks(np.arange(x1, x2, step=data['x_axis']['xticks']))

if 'scientific' in data['x_axis'] and data['x_axis']['scientific'] == 'yes':
  ax.ticklabel_format(style='sci', axis='x', scilimits=(0,0))
if 'scientific' in data['y_axis'] and data['y_axis']['scientific'] == 'yes':
  ax.ticklabel_format(style='sci', axis='y', scilimits=(0,0))

if 'suffix' in data['y_axis'] and data['y_axis']['suffix'] == 'yes':
  formatter = EngFormatter()
  ax.yaxis.set_major_formatter(formatter)

if 'suffix' in data['x_axis'] and data['x_axis']['suffix'] == 'yes':
  formatter = EngFormatter()
  ax.xaxis.set_major_formatter(formatter)

legend_parameters = {}
if 'legend_font_size' in data:
  legend_parameters['fontsize'] = data['legend_font_size']
if 'legend_frame_alpha' in data:
  legend_parameters['framealpha'] = data['legend_frame_alpha']
if 'legend_fancy_box' in data:
  legend_parameters['fancybox'] = data['legend_fancy_box']

# plot the legend
if 'legend' in data:
  plt.legend(loc=data['legend'], **legend_parameters)
else:
  plt.legend(prop=legend_parameters)

if 'textbox' in data:
  if 'text' in data['textbox'] and 'x' in data['textbox'] and 'font_size' in data['textbox'] and 'y' in data['textbox']:
    props = dict(boxstyle='round', facecolor='white', alpha=0.5, ec='lightgray')
    ax.text(data['textbox']['x'],data['textbox']['y'], data['textbox']['text'], 
            transform=ax.transAxes, fontsize=data['textbox']['font_size'], verticalalignment='top', bbox=props)

# print label distribution
if args.storeplot != "":
  plt.tight_layout()
  plt.savefig(args.storeplot)
else:
  # show the plot
  plt.show()