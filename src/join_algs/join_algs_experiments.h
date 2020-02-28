// The MIT License (MIT)
// Copyright (c) 2017 Thomas Huetter
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/// \file src/join_algs/join_algs_experiments.h
///
/// \details
/// Implements an experimental environment that executes algorithms in located in
/// in the folder external/.

#ifndef JOIN_ALGS_EXPERIMENTS_H
#define JOIN_ALGS_EXPERIMENTS_H

#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <unordered_set>
#include "timing.h"
#include "node.h"
#include "string_label.h"
#include "bracket_notation_parser.h"
#include "t_join.h"
#include "tang_join.h"
#include "naive_join.h"
#include "lh_join.h"
#include "dh_join.h"
#include "ldh_join.h"
#include "bb_join.h"
#include "histo_join.h"
#include "unit_cost_model.h"
#include "label_set_converter.h"
#include "binary_tree_converter.h"
#include "zhang_shasha.h"
#include "touzet.h"
#include "greedy_ub.h"
#include "apted.h"
#include "guha.h"

std::vector<std::vector<unsigned int>> reference_sets_;

#endif // JOIN_ALGS_EXPERIMENTS_H
