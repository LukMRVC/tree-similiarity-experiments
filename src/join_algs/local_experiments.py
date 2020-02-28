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
    File name: local_experiments.py
    Author: Thomas Huetter
    Program: Wrapper script to call ../build/ted-join-experiments. Instead 
             of writing to the database, the output is written to stdout in 
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