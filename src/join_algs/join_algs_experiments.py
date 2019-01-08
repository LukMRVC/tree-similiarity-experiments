#!/usr/bin/env python3

# The MIT License (MIT)
# Copyright (c) 2017 Thomas Huetter.
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

'''
Program: Wrapper script to call ./experiments.
'''

import sys
import argparse
import subprocess
import json
import psycopg2
import socket
import os
from psycopg2 import sql
from multiprocessing import Queue, Process
from datetime import datetime

# TODO: Fix the paths depending on where is the script executed from.
# Everything is executed from the project's root.
binary_name = "build/ted-join-experiments"
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

def get_respository_version(repository_root):
    popen = subprocess.Popen(["git", "rev-parse", "HEAD"], stdout=subprocess.PIPE, cwd=repository_root)
    stdoutdata, stderrdata = popen.communicate()
    if popen.returncode != 0:
        raise Exception("non-zero return code: %d" % popen.returncode)
    return bytes.decode(stdoutdata).strip()

def get_algorithm_version():
    return get_respository_version(algorithms_repository_path)

def get_experiments_version():
    return get_respository_version(".")

def get_hostname():
    return socket.gethostname()

# http://initd.org/psycopg/docs/sql.html#module-psycopg2.sql
def store_result(service, table_name, values_dict):
    # Connect to database.
    db = psycopg2.connect("service=" + service)
    # Open a cursor to perform database operations
    cur = db.cursor()
    attributes = values_dict.keys()
    query = sql.SQL("INSERT INTO {} ({}) VALUES ({})").format(
        sql.Identifier(table_name),
        sql.SQL(', ').join(map(sql.Identifier, attributes)),
        sql.SQL(', ').join(map(sql.Placeholder, attributes))
    )
    cur.execute(query, values_dict)
    db.commit()
    # Close the cursor.
    cur.close()
    # Close communication with the database.
    db.close()

def print_result(table_name, values_dict):
    algorithm_params = {"algorithm" : table_name}
    values_dict.update(algorithm_params)
    print(values_dict)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--config', type=str, required=True, 
        help="Path to experiments config file.")
    parser.add_argument('--dataset_path', type=str, required=True, 
        help="Path to the root directory of the datasets.")
    parser.add_argument('--service', type=str, 
        help="Service name for db connection. Specify this parameter to store the results in a database.")
    args = parser.parse_args()

    # Fixed values for this execution - passed to all algorithms.
    fixed_values = {
        "experiments_timestamp" : get_experiments_timestamp(),
        "algorithm_version" : get_algorithm_version(),
        "experiments_version" : get_experiments_version(),
        "hostname" : get_hostname()
    }

    data = json.load(open(args.config))
    for a in data['algorithms']:
        for ds in data['datasets']:
            for t in data['thresholds']:
                # dataset path is composed of argument dataset_path and the path specified in the config
                d = args.dataset_path + ds
                path, filename = os.path.split(d)
                experiment_params = {
                    "dataset_filename" : filename,
                    "threshold" : t
                }
                # build command that needs to be executed
                cmd = []
                # call binary
                if a['name'] == 't_join' or a['name'] == 'tang_join' or a['name'] == 'leaf_distance_histogram_join' or a['name'] == 'label_histogram_join' or a['name'] == 'degree_histogram_join' or a['name'] == 'binary_branches_join' or a['name'] == 'histogram_join':
                    algorithm_params = {
                        "verification_algorithm" : a['verification_algorithm'],
                        "upperbound" : a['upperbound']
                    }
                    cmd.extend((binary_name, d, str(t), a['name'], a['verification_algorithm'], "", a['upperbound']))
                elif a['name'] == 'naive_join':
                    algorithm_params = {
                        "verification_algorithm" : a['verification_algorithm']
                    }
                    cmd.extend((binary_name, d, str(t), a['name'], a['verification_algorithm'], "", a['upperbound']))
                cmd_output = get_stdout_cmd(cmd).strip()
                result_data = json.loads(cmd_output.decode('utf-8'))
                result_data.update(fixed_values)
                result_data.update(experiment_params)
                result_data.update(algorithm_params)
                if args.service:
                    store_result(args.service, a['name'], result_data)
                print_result(a['name'], result_data)


if __name__ == "__main__":
    main()