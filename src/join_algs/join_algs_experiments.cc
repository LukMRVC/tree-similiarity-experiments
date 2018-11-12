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

/// \file src/join_algs/join_algs_experiments.cc
///
/// \details
/// Implements an experimental environment that executes algorithms located in
/// the folder external/. Input file, distance threshold and algorithm are
/// passed as commandline arguments.

#include "join_algs_experiments.h"

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_naive_join(std::vector<node::Node<Label>>& trees_collection, double distance_threshold) {
  // Initialize join algorithm
  join::NaiveJoin<Label, CostModel, VerificationAlgorithm> nsj;

  // Initialized Timing object
  Timing timing;
  Timing::Interval * naive_join = timing.create_enroll("NaiveJoin");
  // Start timing
  naive_join->start();

  // Verify all computed join candidates and return the join result
  std::vector<join::JoinResultElement> join_result_nsj =
      nsj.execute_join(trees_collection, distance_threshold);

  // Stop timing
  naive_join->stop();



  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: join_result_nsj) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            distance_threshold);
    if(ted_value <= distance_threshold)
      optimum_result.emplace_back(pair.tree_id_1, pair.tree_id_2, ted_value);

    // Sum up all number of subproblems
    sum_subproblem_optimum += ted_algorithm.get_subproblem_count();
  }

  // Stop timing
  optimum->stop();

  // Write timing
  std::cout << "\"sum_subproblem_optimum\": " << sum_subproblem_optimum << ", ";
  std::cout << "\"optimum_time\": " << optimum->getfloat() << ", ";


  // Write timing and number of result pairs
  int num_trees = trees_collection.size();
  std::cout << "\"verification_candidates\" : " << (num_trees*num_trees-num_trees)/2 << ", ";
  std::cout << "\"verification_time\" : " << naive_join->getfloat() << ", ";
  std::cout << "\"sum_subproblems\": " << nsj.get_subproblem_count() << ", ";
  std::cout << "\"join_result_size\" : " << join_result_nsj.size() << "}" << std::endl;
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_t_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::TJoin<Label, CostModel, VerificationAlgorithm> tj;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::pair<unsigned int, std::vector<label_set_converter::LabelSetElement>>> sets_collection;
    tj.convert_trees_to_sets(trees_collection, sets_collection);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * retCand = timing.create_enroll("RetrieveCandidates");
      // Start timing
      retCand->start();

      // Retrieve candidates for tjoin's candidate index
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      tj.retrieve_candidates(sets_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        // ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        // Send candidates to the result if their label guided mapping upper 
        // bound is below the threshold.
        tj.upperbound(trees_collection, join_candidates,
                      join_result, distance_threshold);

        // Stop timing
        greedyub->stop();

        // Write timing
        std::cout << "\"upperbound_time\": " << greedyub->getfloat() << ", ";
        std::cout << "\"upperbound_pruned\": " << join_result.size() << ", ";
      } else {
        std::cout << "\"upperbound_time\": 0" << ", ";
        std::cout << "\"upperbound_pruned\": 0" << ", ";
      }

      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      tj.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << tj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << tj.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << tj.get_subproblem_count() << ", ";
    std::cout << "\"join_result_size\": " << join_result.size() << ", ";
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: join_result) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            distance_threshold);
    if(ted_value <= distance_threshold)
      optimum_result.emplace_back(pair.tree_id_1, pair.tree_id_2, ted_value);

    // Sum up all number of subproblems
    sum_subproblem_optimum += ted_algorithm.get_subproblem_count();
  }

  // Stop timing
  optimum->stop();

  // Write timing
  std::cout << "\"sum_subproblem_optimum\": " << sum_subproblem_optimum << ", ";
  std::cout << "\"optimum_time\": " << optimum->getfloat() << "}" << std::endl;
}

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_tang_join(std::vector<node::Node<Label>>& trees_collection, 
    std::string upperbound, double distance_threshold) {
  // Initialize join algorithm
  join::TangJoin<Label, CostModel, VerificationAlgorithm> tangj;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_binary_tree = timing.create_enroll("TreeToBinaryTree");
    // Start timing
    tree_to_binary_tree->start();

    // Convert trees to sets and get the result.
    std::vector<node::BinaryNode<Label>> binary_tree_collection;
    tangj.convert_trees_to_binary_trees(trees_collection, binary_tree_collection);

    // Stop timing
    tree_to_binary_tree->stop();

    // Write timing
    std::cout << "\"tree_to_binary_tree_time\": " << tree_to_binary_tree->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * partsj = timing.create_enroll("RetrieveCandidates");
      // Start timing
      partsj->start();

      // Retrieve candidates for tang's candidate index
      std::unordered_set<std::pair<unsigned int, unsigned int>, join::hashintegerpair> join_candidates;
      tangj.retrieve_candidates(binary_tree_collection, join_candidates, distance_threshold);

      // Stop timing
      partsj->stop();

      // Write timing
      std::cout << "\"index_time\": " << partsj->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::unordered_set<std::pair<unsigned int, unsigned int>, join::hashintegerpair>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);

          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            it = join_candidates.erase(it);
          }
          else {
            ++it;
          }
        }

        // Stop timing
        greedyub->stop();

        // Write timing
        std::cout << "\"upperbound_time\": " << greedyub->getfloat() << ", ";
        std::cout << "\"upperbound_pruned\": " << join_result.size() << ", ";
      } else {
        std::cout << "\"upperbound_time\": 0" << ", ";
        std::cout << "\"upperbound_pruned\": 0" << ", ";
      }

      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      tangj.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << tangj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << tangj.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << tangj.get_subproblem_count() << ", ";
    std::cout << "\"join_result_size\": " << join_result.size() << ", ";
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: join_result) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            distance_threshold);
    if(ted_value <= distance_threshold)
      optimum_result.emplace_back(pair.tree_id_1, pair.tree_id_2, ted_value);

    // Sum up all number of subproblems
    sum_subproblem_optimum += ted_algorithm.get_subproblem_count();
  }

  // Stop timing
  optimum->stop();

  // Write timing
  std::cout << "\"sum_subproblem_optimum\": " << sum_subproblem_optimum << ", ";
  std::cout << "\"optimum_time\": " << optimum->getfloat() << "}" << std::endl;
}

int main(int argc, char** argv) {
  using Label = label::StringLabel;
  using CostModel = cost_model::UnitCostModel<Label>;
  using ZhangShasha = ted::ZhangShasha<Label, CostModel>;
  using Touzet = ted::Touzet<Label, CostModel>;
  using APTED = ted::APTED<Label, CostModel>;

  Timing timing;

  // Write results as JSON object to stdout
  std::cout << "{";
  // Path to file containing the input tree.
  std::string file_path = argv[1];

  // Set distance threshold - maximum number of allowed edit operations.
  double distance_threshold = std::stod(argv[2]);

  // Create the container to store all trees.
  std::vector<node::Node<Label>> trees_collection;

  // Name of the used upperbound
  std::string upperbound = argv[6];

  ////////////////////////////////////////////////////////////////////////
  /// PARSE INPUT
  ////////////////////////////////////////////////////////////////////////
  {
    // Initialized Timing object
    Timing::Interval * parse = timing.create_enroll("Parse");
    // Start timing
    parse->start();

    // Parse the dataset.
    parser::BracketNotationParser bnp;
    bnp.parse_collection(trees_collection, file_path);

    // Stop timing
    parse->stop();

    // Write timing
    std::cout << "\"dataset_parsing_time\": " << parse->getfloat() << ", ";
  }
  ////////////////////////////////////////////////////////////////////////
  /// EXECUTE ALGORITHM
  ////////////////////////////////////////////////////////////////////////

  // Execute algorithm based on input parameters
  if (argv[3] == std::string("t_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_t_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_t_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_t_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if (argv[3] == std::string("tang_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_tang_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_tang_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_tang_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if(argv[3] == std::string("naive_self_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_naive_join<Label, CostModel, ZhangShasha>(trees_collection, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_naive_join<Label, CostModel, Touzet>(trees_collection, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_naive_join<Label, CostModel, APTED>(trees_collection, distance_threshold);
    }
  }

  return 0;
}
