// The MIT License (MIT)
// Copyright (c) 2017 Mateusz Pawlik
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

/// \file src/ted-algs/ted_algs_experiments.cc
///
/// \details
/// Implements an experimental environment that executes ted algorithms.

#include "ted_algs_experiments.h"

struct DataItem {
  unsigned int tree_id_1;
  unsigned int tree_id_2;
  int k;
  unsigned int tree_size_1;
  unsigned int tree_size_2;
  double ted;
  double runtime;
  unsigned long long int subproblems;
  unsigned long long int top_y_updates;
  
  DataItem() {};
  DataItem(unsigned int tid1, unsigned int tid2, int k, unsigned int s1,
      unsigned int s2, double ted, double r, unsigned long long int s,
      unsigned long long int top_y) :
      tree_id_1{tid1},
      tree_id_2{tid2},
      k{k},
      tree_size_1{s1},
      tree_size_2{s1},
      ted{ted},
      runtime{r},
      subproblems{s},
      top_y_updates{top_y}
  {};
  
  std::string to_json_string() {
    std::string output = "{";
    output += "\"tree_id_1\" : " + std::to_string(tree_id_1) + ", ";
    output += "\"tree_id_2\" : " + std::to_string(tree_id_2) + ", ";
    output += "\"k\" : " + std::to_string(k) + ", ";
    output += "\"tree_size_1\" : " + std::to_string(tree_size_1) + ", ";
    output += "\"tree_size_2\" : " + std::to_string(tree_size_2) + ", ";
    // JSON doesn't permit infinity values, but Python's json module parses 'Infinity'.
    // Though, Infinity has to be handled manualy while plotting.
    // Currently, counting the Infinity values is supported.
    output += "\"ted\" : " + (ted == std::numeric_limits<double>::infinity() ? "Infinity" : std::to_string(ted)) + ", ";
    output += "\"subproblems\" : " + std::to_string(subproblems) + ", ";
    output += "\"top_y_updates\" : " + std::to_string(top_y_updates) + ", ";
    output += "\"runtime\" : " + std::to_string(runtime);
    output += "}";
    return output;
  };
  
  std::string to_csv_string() {
    std::string output;
    output += std::to_string(tree_id_1) + "," + std::to_string(tree_id_2) + "," +
        std::to_string(tree_size_1) + "," + std::to_string(tree_size_2) + "," +
        std::to_string(ted) + "," + std::to_string(subproblems) + "," +
        std::to_string(runtime);
    return output;
  };
};

struct AlgorithmItem {
  std::string algorithm_name;
  std::vector<DataItem> data_items;
  
  AlgorithmItem() {};
  AlgorithmItem(std::string a_name) : algorithm_name{a_name} {};
  AlgorithmItem(std::string a_name, std::vector<DataItem> d_items) :
      algorithm_name{a_name},
      data_items{d_items}
  {};
  
  std::string to_json_string() {
    std::string output;
    output += "{\"algorithm_name\" : \"" + algorithm_name +
        "\", \"data_items\" : [";
    for (auto di : data_items) {
      output += di.to_json_string() + ",";
    }
    output.pop_back();
    output += "]}";
    return output;
  };
  
  std::string to_csv_string() {
    std::string output;
    for (auto di : data_items) {
      output += algorithm_name + "," + di.to_csv_string() + "\n";
    }
    output.pop_back();
    return output;
  }
};

struct Experiment {
  std::string dataset_file;
  int k;
  double dataset_parsing_time;
  std::vector<AlgorithmItem> algorithm_executions;
  
  Experiment() {};
  Experiment(std::string dataset_file, int k) :
      dataset_file{dataset_file},
      k{k}
  {};
  
  std::string to_json_string() {
    std::string output;
    output += "{\"dataset_parsing_time\" : " + std::to_string(dataset_parsing_time) +
        ", \"algorithm_executions\" : [";
    for (auto a : algorithm_executions) {
      output += a.to_json_string() + ",";
    }
    output.pop_back();
    output += "]}";
    return output;
  };
  
  std::string to_csv_string() {
    std::string output;
    for (auto a : algorithm_executions) {
      output += a.to_csv_string() + "\n";
    }
    output.pop_back();
    return output;
  };
};

struct MechanismParams {
  unsigned int mechanism;
  unsigned int t1_id;
  unsigned int t2_id;
  unsigned int k_min;
  unsigned int k_max;
  unsigned int k_step;
  
  MechanismParams() {};
  MechanismParams(unsigned int mechanism) :
      mechanism{mechanism}
  {};
  MechanismParams(unsigned int mechanism, unsigned int t1_id, unsigned int t2_id) :
      mechanism{mechanism},
      t1_id{t1_id},
      t2_id{t2_id}
  {};
  MechanismParams(unsigned int mechanism, unsigned int t1_id, unsigned int t2_id,
      unsigned int k_min, unsigned int k_max, unsigned int k_step) :
      mechanism{mechanism},
      t1_id{t1_id},
      t2_id{t2_id},
      k_min{k_min},
      k_max{k_max},
      k_step{k_step}
  {};
};

// Experiment mechanisms.
const unsigned int kOverlappingPairs = 1;
const unsigned int kOneByOne         = 2;
const unsigned int kChoosePair       = 3;
const unsigned int kChoosePairKRange = 4;

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
DataItem execute_ted_alg(const unsigned int t1_id, const unsigned int t2_id, const node::Node<Label>& t1, const node::Node<Label>& t2, const int k) {
  Algorithm a;
  Timing timing;
  Timing::Interval * alg_time = timing.create_enroll("AlgorithmRuntime");
  alg_time->start();
  auto d = (a.*_ted)(t1, t2, k);
  alg_time->stop();
  auto sp = a.get_subproblem_count();
  // auto top_y = a.get_top_y_update_count(); NOTE: Disabled due to unifying ted and ted_k algorithms.
  DataItem di(t1_id, t2_id, k, t1.get_tree_size(), t2.get_tree_size(), d, alg_time->getfloat(), sp, 0);
  return di;
};

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
std::vector<DataItem> execute_overlaping_pairs(std::vector<node::Node<Label>>& trees_collection, MechanismParams& mp, const int k) {
  std::vector<DataItem> execution_results;
  auto tc_start_it = std::begin(trees_collection);
  auto tc_end_it = std::end(trees_collection);
  unsigned int tree_id = 0;
  while (tc_start_it < tc_end_it-1) {
    auto t1 = *tc_start_it;
    auto t2 = *(tc_start_it+1);
    execution_results.push_back(execute_ted_alg<Label, Algorithm, _ted>(tree_id, tree_id+1, t1, t2, k));
    ++tc_start_it;
    ++tree_id;
  }
  return execution_results;
};

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
std::vector<DataItem> execute_one_by_one(std::vector<node::Node<Label>>& trees_collection, MechanismParams& mp, const int k) {
  std::vector<DataItem> execution_results;
  unsigned int tree_id = 0;
  for (auto& t : trees_collection) {
    execution_results.push_back(execute_ted_alg<Label, Algorithm, _ted>(tree_id, tree_id, t, t, k));
    ++tree_id;
  }
  return execution_results;
};

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
std::vector<DataItem> execute_choose_pair(std::vector<node::Node<Label>>& trees_collection, MechanismParams& mp, const int k) {
  std::vector<DataItem> execution_results;
  execution_results.push_back(
      execute_ted_alg<Label, Algorithm, _ted>(mp.t1_id, mp.t2_id,
          trees_collection.at(mp.t1_id), trees_collection.at(mp.t2_id), k
      )
  );
  return execution_results;
};

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
std::vector<DataItem> execute_choose_pair_k_range(std::vector<node::Node<Label>>& trees_collection, MechanismParams& mp, const int k) {
  std::vector<DataItem> execution_results;
  for (unsigned int k_i = mp.k_min; k_i <= mp.k_max; k_i = k_i + mp.k_step) {
    execution_results.push_back(
        execute_ted_alg<Label, Algorithm, _ted>(mp.t1_id, mp.t2_id,
            trees_collection.at(mp.t1_id), trees_collection.at(mp.t2_id), k_i
        )
    );
  }
  return execution_results;
};

template <typename Label, typename Algorithm, double (Algorithm::*_ted)(const node::Node<Label>&, const node::Node<Label>&, const int k)>
std::vector<DataItem> execute_mechanism(std::vector<node::Node<Label>>& trees_collection, MechanismParams& mp, const int k) {
  std::vector<DataItem> execution_results;
  switch(mp.mechanism) {
    case kOverlappingPairs :
      execution_results = execute_overlaping_pairs<Label, Algorithm, _ted>(trees_collection, mp, k);
      break;
    case kOneByOne :
      execution_results = execute_one_by_one<Label, Algorithm, _ted>(trees_collection, mp, k);
      break;
    case kChoosePair :
      execution_results = execute_choose_pair<Label, Algorithm, _ted>(trees_collection, mp, k);
      break;
    case kChoosePairKRange :
      execution_results = execute_choose_pair_k_range<Label, Algorithm, _ted>(trees_collection, mp, k);
      break;
  }
  return execution_results;
};

int main(int argc, char** argv) {
  
  using Label = label::StringLabel;
  using CostModel = cost_model::UnitCostModel<Label>;
  
  // TED algorithms.
  using ZhangShasha = ted::ZhangShasha<Label, CostModel>;
  using APTED = ted::APTED<Label, CostModel>;
  
  // TED-k algorithms.
  using Touzet = ted::Touzet<Label, CostModel>;
  using LabelGuided = ted_ub::GreedyUB<Label, CostModel>;
  
  Timing timing;
  
  // Write results as JSON object to stdout.
  // QUESTION: What's the best way of building a string output?
  
  // Path to file containing the input trees.
  std::string input_file_path;
  
  // Experiment mechanism.
  //
  // Possible execution mechanisms over the input dataset:
  // --overlaping-pairs  First with second, second with third, and so on (default).
  // --one-by-one        Self distance.
  // --choose-pair T1 T2 Choose a specific pair of trees T1 T2 (line numbers in input file, starting from 0).
  MechanismParams mp(kOverlappingPairs);
  
  // Set similarity threshold - maximum number of allowed edit operations.
  // TODO: If no threshold is provided, it should be set to something reasonable (default).
  int similarity_threshold = 0;
  
  // Names of the used TED agorithms (a list).
  //
  // Possible algorithms (command line arguments):
  // --zs    Zhang and Shasha                          zhang_shasha_ted
  // --apted APTED                                     apted_ted
  // --tz    Touzet - basic version                    touzet_ted
  // --tzd   Touzet - depth-based pruning              touzet_ted_depth_pruning
  // --tzs   Touzet - keyroot nodes with set           touzet_ted_kr_loop
  // --tzl   Touzet - keyroot nodes with loop          touzet_ted_kr_set
  // --tzse  Touzet - keyroot nodes with set + e_max   touzet_ted_kr_loop
  // --tzle  Touzet - keyroot nodes with loop + e_max  touzet_ted_kr_set
  // --lg    LabelGuided                               greedy_ub_ted
  bool alg_zs_is_set = false;
  bool alg_apted_is_set = false;
  bool alg_tz_is_set = false;
  bool alg_tzd_is_set = false;
  bool alg_tzs_is_set = false;
  bool alg_tzl_is_set = false;
  bool alg_tzse_is_set = false;
  bool alg_tzle_is_set = false;
  bool alg_lg_is_set = false;
  
  // Output format
  bool output_in_json = false;
  bool output_in_csv = false;
  
  // Parse command line arguments.
  // TODO: Check for possible errors.
  std::vector<std::string> args(argv, argv + argc);
  auto args_start_it = std::begin(args);
  auto args_end_it = std::end(args);
  while (args_start_it != args_end_it) {
    std::string a = *args_start_it;
    if (a == "--input") {
      ++args_start_it;
      input_file_path = *args_start_it;
    }
    if (a == "--threshold") {
      ++args_start_it;
      similarity_threshold = static_cast <int> (std::ceil(std::stod(*args_start_it)));
    }
    if (a == "--zs") {
      alg_zs_is_set = true;
    }
    if (a == "--apted") {
      alg_apted_is_set = true;
    }
    if (a == "--tz") {
      alg_tz_is_set = true;
    }
    if (a == "--tzd") {
      alg_tzd_is_set = true;
    }
    if (a == "--tzs") {
      alg_tzs_is_set = true;
    }
    if (a == "--tzl") {
      alg_tzl_is_set = true;
    }
    if (a == "--tzse") {
      alg_tzse_is_set = true;
    }
    if (a == "--tzle") {
      alg_tzle_is_set = true;
    }
    if (a == "--lg") {
      alg_lg_is_set = true;
    }
    if (a == "--one-by-one") {
      // mechanism_to_execute = kOneByOne;
      mp = MechanismParams(kOneByOne);
    }
    if (a == "--choose-pair") {
      mp = MechanismParams(kChoosePair,
          std::stoi(*(args_start_it+1)),
          std::stoi(*(args_start_it+2))
      );
      args_start_it += 2;
    }
    if (a == "--choose-pair-k-range") {
      mp = MechanismParams(kChoosePairKRange,
          std::stoi(*(args_start_it+1)),
          std::stoi(*(args_start_it+2)),
          std::stoi(*(args_start_it+3)),
          std::stoi(*(args_start_it+4)),
          std::stoi(*(args_start_it+5))
      );
      args_start_it += 5;
    }
    if (a == "--output") {
      ++args_start_it;
      a = *args_start_it;
      if (a == "json") {
        output_in_json = true;
      } else if (a == "csv") {
        output_in_csv = true;
      }
    }
    ++args_start_it;
  }

  Experiment experiment(input_file_path, similarity_threshold);

  // PARSE INPUT
  // The input is parsed once for the entire experiment.
  std::vector<node::Node<Label>> trees_collection;
  {
    Timing::Interval * parse = timing.create_enroll("Parse");
    parse->start();
    parser::BracketNotationParser bnp;
    bnp.parse_collection(trees_collection, input_file_path);
    parse->stop();
    experiment.dataset_parsing_time = parse->getfloat();
  }
  
  // EXECUTE ALGORITHMS
  if (alg_zs_is_set) {
    experiment.algorithm_executions.emplace_back("ZhangShasha",
        execute_mechanism<Label, ZhangShasha, &ZhangShasha::zhang_shasha_ted_k>(trees_collection, mp, similarity_threshold));
  }
  if (alg_apted_is_set) {
    experiment.algorithm_executions.emplace_back("APTED",
        execute_mechanism<Label, APTED, &APTED::apted_ted_k>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tz_is_set) {
    experiment.algorithm_executions.emplace_back("Touzet",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tzd_is_set) {
    experiment.algorithm_executions.emplace_back("TouzetDP",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted_depth_pruning>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tzs_is_set) {
    experiment.algorithm_executions.emplace_back("TouzetKrLoop",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted_kr_loop_no_e_max>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tzl_is_set) {
    experiment.algorithm_executions.emplace_back("TouzetKrSet",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted_kr_set_no_e_max>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tzse_is_set) {
    experiment.algorithm_executions.emplace_back("TouzetKrLoopEmax",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted_kr_loop_e_max>(trees_collection, mp, similarity_threshold));
  }
  if (alg_tzle_is_set) {
    experiment.algorithm_executions.emplace_back("TouzetKrSetEmax",
        execute_mechanism<Label, Touzet, &Touzet::touzet_ted_kr_set_e_max>(trees_collection, mp, similarity_threshold));
  }
  if (alg_lg_is_set) {
    experiment.algorithm_executions.emplace_back("LabelGuided",
        execute_mechanism<Label, LabelGuided, &LabelGuided::greedy_ub_ted>(trees_collection, mp, similarity_threshold));
  }
  
  // OUTPUT RESULTS
  if (output_in_json) {
    std::cout << experiment.to_json_string() << std::endl;
  }
  if (output_in_csv) {
    std::cout << experiment.to_csv_string() << std::endl;
  }
  
  return 0;
}
