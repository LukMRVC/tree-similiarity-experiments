#!/usr/bin/env python
'''
    File name: local_experiments.py
    Author: Thomas Huetter
    Program: Wrapper script to call ../build/ted-join-experiments. Instead 
             of writing tothe database, the output is written to stdout in 
             json format. Called from /src/.
'''

import sys
import argparse
import subprocess
import json

# execute a command and return stdout
def get_stdout_cmd(callargs):
    popen = subprocess.Popen(callargs, stdout=subprocess.PIPE)
    stdoutdata, stderrdata = popen.communicate()
    if popen.returncode != 0:
        raise Exception("non-zero return code: %d" % popen.returncode)
    return stdoutdata

# path+filename of the binary
binary_name = "../build/ted-join-experiments"

# parse input argurments
parser = argparse.ArgumentParser()
parser.add_argument("--inputfiles", nargs='+', type=str, 
  help="path to input files containing line seperated trees in bracket notation")
parser.add_argument("--thresholds", nargs='+', type=float, 
  help="thresholds for the number of edit operations in the tree edit distance")
parser.add_argument("--algorithms", nargs='+', type=str, 
  help="one or more tree similarity join algorithms that should be used")
args = parser.parse_args()

# execute binary for all possible input configurations
for file in args.inputfiles:
  for threshold in args.thresholds:
    for algorithm in args.algorithms:
      #build command that needs to be executed
      cmd = []
      # call binary
      cmd.extend((binary_name, file, str(threshold), algorithm))
      # get output of the c++ program
      cmd_output = get_stdout_cmd(cmd).strip()
      # print utf-8 encoded json objects
      print(cmd_output.decode('utf-8'))