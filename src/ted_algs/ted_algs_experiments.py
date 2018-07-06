#!/usr/bin/python3
'''
    File name: experiments.py
    Author: Thomas Huetter, Mateusz Pawlik
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

def print_result(table_name, values_dict):
    algorithm_params = {"algorithm" : table_name}
    values_dict.update(algorithm_params)
    print(values_dict)

parser = argparse.ArgumentParser()
parser.add_argument(
    type=str,
    dest='config_filename',
    help="Path to experiments config file."
)
args = parser.parse_args()

# Fixed values for this execution - passed to all algorithms.
fixed_values = {
    "experiments_timestamp" : get_experiments_timestamp(),
    "algorithm_version" : get_algorithm_version(),
    "experiments_version" : get_experiments_version(),
    "hostname" : get_hostname()
}

data = json.load(open(args.config_filename))
for d in data['datasets']:
    for t in data['thresholds']:
        path, filename = os.path.split(d)
        experiment_params = {
            "dataset_filename" : filename,
            "threshold" : t
        }
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
            data['mechanism'],
        ])
        cmd.extend(data['algorithms'])
        print(cmd)
        
        cmd_output = get_stdout_cmd(cmd).strip()
        result_data = json.loads(cmd_output.decode('utf-8'))
        result_data.update(fixed_values)
        result_data.update(experiment_params)
        # result_data.update(algorithm_params)
        # store_result(a['name'], result_data)
        # print_result(a['name'], result_data)
        print(result_data)

