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

import sys
import argparse
import subprocess
import json
import psycopg2
import socket
import os
from psycopg2 import sql
from psycopg2 import extras
from multiprocessing import Queue, Process
from datetime import datetime

# README
# 
# This script executes ted experiments and stores the results in the database.
# 
# It ensures that the experiments binary is executed once for each combination
# of dataset, algorithm, threshold, and mechanism.

# TODO: Fix the paths depending on where is the script executed from.
# Everything is executed from the project's root.
binary_name = "build/ted-algs-experiments"
algorithms_repository_path = "external/tree-similarity/"

# execute a command and return stdout
def get_stdout_cmd(callargs):
    popen = subprocess.Popen(callargs, stdout=subprocess.PIPE)
    stdoutdata, stderrdata = popen.communicate()
    if popen.returncode != 0:
        raise Exception("non-zero return code: %d" % popen.returncode)
    return stdoutdata

def get_experiments_timestamp():
    # INSERT INTO some_table
    # (ts_column)
    # VALUES
    # (TIMESTAMP '2011-05-16 15:36:38');
    return datetime.now()

def get_respository_commit(repository_root):
    popen = subprocess.Popen(["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE, cwd=repository_root)
    stdoutdata, stderrdata = popen.communicate()
    if popen.returncode != 0:
        raise Exception("non-zero return code: %d" % popen.returncode)
    return bytes.decode(stdoutdata).strip()

def get_algorithm_source_commit():
    return get_respository_commit(algorithms_repository_path)

def get_experiments_source_commit():
    return get_respository_commit(".")

def get_hostname():
    return socket.gethostname()

def print_result(table_name, values_dict):
    algorithm_params = {"algorithm" : table_name}
    values_dict.update(algorithm_params)
    print(values_dict)

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def store_ted_experiment_params(service, params_dict):
    # Connect to database.
    db = psycopg2.connect("service=" + service)
    # Open a cursor to perform database operations
    cur = db.cursor()
    attributes = params_dict.keys()
    query = sql.SQL("INSERT INTO ted_experiment_params ({}) VALUES ({}) RETURNING ted_experiment_params_id").format(
        sql.SQL(', ').join(map(sql.Identifier, attributes)),
        sql.SQL(', ').join(map(sql.Placeholder, attributes))
    )
    # print(query.as_string(cur))
    cur.execute(query, params_dict)
    new_params_id = cur.fetchone()[0]
    db.commit()
    # Close the cursor.
    cur.close()
    # Close communication with the database.
    db.close()
    return new_params_id

# Store multiple-rows results at once.
def store_multirow_results(service, table_name, results, new_params_id, store_threshold, store_has_mapping):
    db = psycopg2.connect("service=" + service)
    cur = db.cursor()
    attributes = list(results[0].keys())
    # Remove the threshold if algorithm doesn't use it (APTED, ZS).
    if not store_threshold:
        attributes.remove('ted_threshold')
    # Remove the has_ted_mapping if algorithm doesn't use it (LG).
    if not store_has_mapping:
        attributes.remove('has_ted_mapping')
    # Add ted_experiment_params FK to attributes.
    # attributes.append('ted_experiment_params_id')
    # Prepare the query.
    query = sql.SQL("INSERT INTO {} ({}) VALUES %s").format(
        sql.Identifier(table_name),
        sql.SQL(', ').join(map(sql.Identifier, attributes + ['ted_experiment_params_id']))
    )
    # print(query.as_string(cur))
    extras.execute_values(
        cur,
        query.as_string(cur),
        results,
        template='(' + sql.SQL(', ').join(map(sql.Placeholder, attributes)).as_string(cur) + ',' + str(new_params_id) + ')'
    )
    
    # TODO: Dump data in case of failed INSERT.
    
    db.commit()
    cur.close()
    db.close()

parser = argparse.ArgumentParser()
parser.add_argument(
    type=str,
    dest='config_filename',
    help="Path to experiments config file."
)
parser.add_argument(
    type=str,
    dest='dataset_path',
    help="Path to the root directory of the datasets (with trailing slash)."
)
parser.add_argument(
    type=str,
    dest='service_name',
    help="Service name to use from '~/.pg_service.conf' file."
)
parser.add_argument(
    '--store',
    dest = 'store',
    action = 'store_true',
    help = 'Set to store results in the database (default: not set).'
)
parser.add_argument(
    '--debug',
    dest = 'debug',
    action = 'store_true',
    help = 'Set to print experiments commands that will be executed (default: not set).'
)
args = parser.parse_args()

# Fixed values for this execution - passed to all algorithms.
ted_experiment_params = {
    "ted_experiment_timestamp" : get_experiments_timestamp(),
    "algorithm_source_commit" : get_algorithm_source_commit(),
    "experiments_source_commit" : get_experiments_source_commit(),
    "hostname" : get_hostname()
}

ted_algs = ["--apted", "--zs", "--sed", "--cted"]
tedk_algs = ["--tz", "--tzd"]
tedub_algs = ["--lg",]

table_names = {
    '--apted' : 'ted_apted',
    '--zs' : 'ted_zhangshasha',
    '--sed' : 'ted_sed',
    '--cted' : 'ted_cted',
    '--tz' : 'tedk_touzet',
    '--tzd' : 'tedk_touzetd',
    '--lg' : 'tedk_labelguided'
}

data = json.load(open(args.config_filename))
for a in data['algorithms']:
    for d in data['datasets']:
        for t in data['thresholds']:
            path, filename = os.path.split(d)
            ted_experiment_params.update({
                "dataset_filename" : filename
            })
            print("EXECUTING: " + filename + ";" + a + ";" + str(t))
            
            # build command that needs to be executed
            cmd = []
            cmd.extend([
                binary_name,
                '--input', str(args.dataset_path + d),
                '--output' , 'json',
                '--threshold', str(t)
            ])
            if 'mechanism' in data:
                cmd.extend([data['mechanism'],])
            cmd.extend([a,])
            
            # TODO: Verify if an experiment like that has been already executed
            #       and how many values it has.
            
            cmd_output = ""
            result_data = []
            
            if args.debug:
                print(cmd)
            else:
                cmd_output = get_stdout_cmd(cmd).strip()
                result_data = json.loads(cmd_output.decode('utf-8'))
            
            if not args.store and not args.debug:
                print("RESULTS:")
                print(cmd_output)
            
            if args.store and not args.debug:
                print("STORING ...")
                new_params_id = store_ted_experiment_params(args.service_name, ted_experiment_params)
                store_multirow_results(
                    args.service_name,
                    table_names[a],
                    result_data['algorithm_executions'][0]['data_items'],
                    new_params_id,
                    (a in tedk_algs or a in tedub_algs),
                    (a in tedk_algs)
                )
            
            # TODO: Handle errors in storing.
            
            # non-tedk algorithms should be executed once disregarding the thresholds
            if a in ted_algs:
                break
