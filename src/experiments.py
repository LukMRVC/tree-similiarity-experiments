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

from multiprocessing import Queue, Process

# execute a command and return stdout
def get_stdout_cmd(callargs):
    popen = subprocess.Popen(callargs, stdout=subprocess.PIPE)
    stdoutdata, stderrdata = popen.communicate()
    if popen.returncode != 0:
        raise Exception("non-zero return code: %d" % popen.returncode)
    return stdoutdata

binary_name = "../build/ted-join-experiments"

parser = argparse.ArgumentParser()
parser.add_argument("--inputfiles", nargs='+', type=str, help="path to input file containing trees in bracket notation (line seperated)")
parser.add_argument("--thresholds", nargs='+', type=float, help="threshold for the number of edit operations in the tree edit distance")
parser.add_argument("--algorithms", nargs='+', type=str, help="one or more tree similarity join algorithms that should be used")

args = parser.parse_args()

# execute binary for all possible input configurations
for file in args.inputfiles:
  for threshold in args.thresholds:
    for algorithm in args.algorithms:
      #build command that needs to be executed
      cmd = []
      # call binary
      cmd.extend((binary_name, file, str(threshold), algorithm))
      print(' '.join(map(str,cmd)) + "\n")
      cmd_output = get_stdout_cmd(cmd).strip()
      json_data = json.loads(cmd_output)
      for key, value in json_data.items():
        print(key + "=" + str(value))

      print("")







