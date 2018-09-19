#!/usr/bin/python3

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
algorithms_repository_path = "external/tree-similarity-private/"

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
def store_result(table_name, values_dict):
    # Connect to database.
    db = psycopg2.connect("service=ted-exp-local-test")
    # Open a cursor to perform database operations
    cur = db.cursor()
    attributes = values_dict.keys()
    query = sql.SQL("INSERT INTO {} ({}) VALUES ({})").format(
        sql.Identifier(table_name),
        sql.SQL(', ').join(map(sql.Identifier, attributes)),
        sql.SQL(', ').join(map(sql.Placeholder, attributes))
    )
    print(query.as_string(cur))
    cur.execute(query, values_dict)
    db.commit()
    # Close the cursor.
    cur.close()
    # Close communication with the database.
    db.close()

def store_multirow_results(table_name):
    values = [
        {'ted': 1.0,
         'tree_id_1': 0,
         'subproblems': 1,
         'tree_id_2': 1,
         'tree_size_2': 1,
         'tree_size_1': 1,
         'runtime': 0.026643},
        {'ted': 1.0,
         'tree_id_1': 1,
         'subproblems': 1,
         'tree_id_2': 2,
         'tree_size_2': 1,
         'tree_size_1': 1,
         'runtime': 0.015511},
        {'ted': 1.0,
         'tree_id_1': 2,
         'subproblems': 2,
         'tree_id_2': 3,
         'tree_size_2': 2,
         'tree_size_1': 2,
         'runtime': 0.021519}
    ]
    db = psycopg2.connect("service=ted-exp-local-test")
    cur = db.cursor()
    attributes = values[0].keys()
    query = sql.SQL("INSERT INTO {} ({}) VALUES %s").format(
        sql.Identifier(table_name),
        sql.SQL(', ').join(map(sql.Identifier, attributes))
    )
    # query = "INSERT INTO ted_apted (tree_id_1, subproblems, ted, runtime, tree_size_2, tree_id_2, tree_size_1) VALUES %s"
    print(query.as_string(cur))
    psycopg2.extras.execute_values(
        cur,
        query.as_string(cur),
        values,
        template='(' + sql.SQL(', ').join(map(sql.Placeholder, attributes)).as_string(cur) + ')'
    )
    db.commit()
    cur.close()
    db.close()

parser = argparse.ArgumentParser()
parser.add_argument(
    type=str,
    dest='config_filename',
    help="Path to experiments config file."
)
args = parser.parse_args()

# Fixed values for this execution - passed to all algorithms.
ted_experiment_params = {
    "ted_experiment_timestamp" : get_experiments_timestamp(),
    "algorithm_source_commit" : get_algorithm_source_commit(),
    "experiments_source_commit" : get_experiments_source_commit(),
    "hostname" : get_hostname()
}

ted_algs = ["--apted", "--zs"]

data = json.load(open(args.config_filename))
for a in data['algorithms']:
    for d in data['datasets']:
        for t in data['thresholds']:
            path, filename = os.path.split(d)
            ted_experiment_params.update({
                "dataset_filename" : filename,
                "ted_threshold" : t
            })
            print(ted_experiment_params)
            
            # build command that needs to be executed
            cmd = []
            # call binary
            # ted-algs-experiments
            # --input
            # ~/temp/programming/RTED/branches/tods-experiments/datasets/artificial/zz/zz-200-2000.dataset
            # --tz --tzd --tzs --tzse --tzl --tzle --lg
            # --one-by-one
            # --threshold 20 --output json
            cmd.extend([
                binary_name,
                '--input', str(d),
                '--output' , 'json',
                '--threshold', str(t),
            ])
            if 'mechanism' in data:
                cmd.extend([data['mechanism'],])
            cmd.extend([a,])
            print(cmd)

            cmd_output = get_stdout_cmd(cmd).strip()
            result_data = json.loads(cmd_output.decode('utf-8'))
            # # result_data.update(algorithm_params)
            # # store_result(a['name'], result_data)
            # # print_result(a['name'], result_data)
            # print(result_data)
                        
            print(result_data['algorithm_executions'][0]['data_items'])
            
            store_multirow_results("ted_apted")
            
            # TODO: Handle inf correctly.
            
            # TODO: Store ted_experiment_params and retrieve its id.
            
            # TODO: Store the results.
            
            # non-tedk algorithms should be executed once disregarding the thresholds
            if a in ted_algs:
                break
