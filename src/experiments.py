#!/usr/bin/env python
'''
    File name: experiments.py
    Author: Thomas Huetter
    Program: Wrapper script to call ./experiments.
'''

import sys
import argparse
import subprocess
import json
import psycopg2
from multiprocessing import Queue, Process
from datetime import datetime

# TODO: Fix the paths depending on where is the script executed from.
binary_name = "../build/ted-join-experiments"
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

def store_naive_self_join_result(db, cur, values):
    # cur.execute("INSERT INTO naive_self_join VALUES ();", (,))
    cur.execute("INSERT INTO naive_self_join( \
        experiments_version, \
        experiments_timestamp, \
        dataset_filename, \
        dataset_parsing_time, \
        algorithm_version, \
        threshold, \
        result_set_size, \
        verification_candidates, \
        verification_time) \
        VALUES ( \
        %(experiments_version)s, \
        %(experiments_timestamp)s, \
        %(dataset_filename)s, \
        %(dataset_parsing_time)s, \
        %(algorithm_version)s, \
        %(threshold)s, \
        %(result_set_size)s, \
        %(verification_candidates)s, \
        %(verification_time)s)", \
        values
    )
    db.commit()


parser = argparse.ArgumentParser()
parser.add_argument(
    "--datasets",
    nargs='+',
    type=str,
    help="Filenames of datasets containing trees in bracket notation \
          (line seperated)."
)
parser.add_argument(
    "--path-to-datasets",
    type=str,
    help="Path to datasets."
)
parser.add_argument(
    "--thresholds",
    nargs='+',
    type=float,
    help="Threshold values for the number of edit operations in the tree edit \
          distance.")
parser.add_argument(
    "--algorithms",
    nargs='+',
    type=str,
    help="One or more tree similarity join algorithms that should be used."
)
parser.add_argument(
    '--connection-string',
    type=str,
    dest='conn_string',
    action='store',
    help="Specify a custom database connection string (default: empty string)",
    default="")

args = parser.parse_args()

# Fixed values for this execution - passed to all algorithms.
fixed_values = {
    "experiments_timestamp" : get_experiments_timestamp(),
    "algorithm_version" : get_algorithm_version(),
    "experiments_version" : get_experiments_version()
}

example_parameters_set = {
    "dataset_filename" : "dataset",
    "threshold" : 1
}

example_naive_join_result = '{ \
    "dataset_parsing_time" : 10, \
    "result_set_size" : 100, \
    "verification_candidates" : 100, \
    "verification_time" : 10 \
}'

example_values_to_store = json.loads(example_naive_join_result)
example_values_to_store.update(example_parameters_set)
example_values_to_store.update(fixed_values)

print(example_values_to_store)

# Connect to database.
db = psycopg2.connect("")
# Open a cursor to perform database operations
cur = db.cursor()

store_naive_self_join_result(db, cur, example_values_to_store)

# Close the cursor.
cur.close()
# Close communication with the database.
db.close()

# # execute binary for all possible input configurations
# for file in args.inputfiles:
#   for threshold in args.thresholds:
#     for algorithm in args.algorithms:
#       #build command that needs to be executed
#       cmd = []
#       # call binary
#       cmd.extend((binary_name, file, str(threshold), algorithm))
#       print(' '.join(map(str,cmd)) + "\n")
#       cmd_output = get_stdout_cmd(cmd).strip()
#       json_data = json.loads(cmd_output)
#       for key, value in json_data.items():
#         print(key + "=" + str(value))
#
#       print("")
