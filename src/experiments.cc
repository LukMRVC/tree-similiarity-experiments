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

/// \file src/experiments.cc
///
/// \details
/// Implements an experimental environment that executes algorithms in located in
/// in the folder external/. Input file, similarity threshold and algorithm are
/// passed as commandline arguments.

#include "experiments.h"

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_naive_self_join(std::vector<node::Node<Label>>& trees_collection, double similarity_threshold) {
  // Initialize allpairs baseline
  join::NaiveSelfJoin<Label, CostModel, VerificationAlgorithm> nsj;

  // Initialized Timing object
  Timing timing;
  Timing::Interval * naive_join = timing.create_enroll("NaiveJoin");
  // Start timing
  naive_join->start();

  // Verify all computed join candidates and return the join result
  std::vector<join::JoinResultElement> result_set_nsj =
      nsj.execute_join(trees_collection, similarity_threshold);

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

  for(auto pair: result_set_nsj) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            similarity_threshold);
    if(ted_value <= similarity_threshold)
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
  std::cout << "\"result_set_size\" : " << result_set_nsj.size() << "}" << std::endl;
}

template <typename Label, typename CostModel, typename SimilarityFunction, typename VerificationAlgorithm>
void execute_allpairs_self_join(std::vector<node::Node<Label>>& trees_collection, double similarity_threshold) {
  // Initialize allpairs baseline
  join::AllpairsGenericSelfJoin<Label, CostModel, SimilarityFunction, VerificationAlgorithm> absj;
  Timing timing;
  std::vector<join::JoinResultElement> result_set;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::vector<unsigned int>> sets_collection;
    absj.convert_trees_to_sets(trees_collection, sets_collection);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * allpairs = timing.create_enroll("Allpairs");
      // Start timing
      allpairs->start();

      // Compute candidate for the join with the allpairs algorithm
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      absj.get_join_candidates(sets_collection, join_candidates, similarity_threshold);

      // Stop timing
      allpairs->stop();

      // Write timing
      std::cout << "\"filter_time\": " << allpairs->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";


      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      absj.verify_candidates(trees_collection, join_candidates,
                             result_set, similarity_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"filter_verification_candidates\": " << absj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"sum_subproblems\": " << absj.get_subproblem_count() << ", ";
    std::cout << "\"result_set_size\": " << result_set.size() << ", ";
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: result_set) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            similarity_threshold);
    if(ted_value <= similarity_threshold)
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

template <typename Label, typename CostModel, typename SimilarityFunction, typename VerificationAlgorithm>
void execute_allpairs_multiset_baseline_self_join(std::vector<node::Node<Label>>& trees_collection, double similarity_threshold) {
  // Initialize allpairs baseline
  join::AllpairsMultisetBaselineSelfJoin<Label, CostModel, SimilarityFunction, VerificationAlgorithm> absj;
  Timing timing;
  std::vector<join::JoinResultElement> result_set;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::pair<unsigned int, std::vector<tree_to_set_converter_multiset_baseline::SetElement>>> sets_collection;
    absj.convert_trees_to_sets(trees_collection, sets_collection);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * allpairs = timing.create_enroll("Allpairs");
      // Start timing
      allpairs->start();

      // Compute candidate for the join with the allpairs algorithm
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      absj.get_join_candidates(sets_collection, join_candidates, similarity_threshold);

      // Stop timing
      allpairs->stop();

      // Write timing
      std::cout << "\"filter_time\": " << allpairs->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";


      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      absj.verify_candidates(trees_collection, join_candidates,
                             result_set, similarity_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"filter_verification_candidates\": " << absj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"sum_subproblems\": " << absj.get_subproblem_count() << ", ";
    std::cout << "\"result_set_size\": " << result_set.size() << ", ";
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: result_set) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            similarity_threshold);
    if(ted_value <= similarity_threshold)
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

template <typename Label, typename CostModel, typename SimilarityFunction, typename VerificationAlgorithm>
void execute_allpairs_multiset_dsf_self_join(std::vector<node::Node<Label>>& trees_collection, double similarity_threshold) {
  // Initialize allpairs baseline
  join::AllpairsMultisetDSFSelfJoin<Label, CostModel, SimilarityFunction, VerificationAlgorithm> absj;
  Timing timing;
  std::vector<join::JoinResultElement> result_set;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::pair<unsigned int, std::vector<tree_to_set_converter_multiset::SetElement>>> sets_collection;
    absj.convert_trees_to_sets(trees_collection, sets_collection);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * allpairs = timing.create_enroll("Allpairs");
      // Start timing
      allpairs->start();

      // Compute candidate for the join with the allpairs algorithm
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      absj.get_join_candidates(sets_collection, join_candidates, similarity_threshold);

      // Stop timing
      allpairs->stop();

      // Write timing
      std::cout << "\"filter_time\": " << allpairs->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";


      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      absj.verify_candidates(trees_collection, join_candidates,
                             result_set, similarity_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"filter_verification_candidates\": " << absj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"sum_subproblems\": " << absj.get_subproblem_count() << ", ";
    std::cout << "\"result_set_size\": " << result_set.size() << ", ";
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();

  VerificationAlgorithm ted_algorithm;
  std::vector<join::JoinResultElement> optimum_result;
  unsigned long long int sum_subproblem_optimum = 0;

  for(auto pair: result_set) {
    double ted_value = ted_algorithm.verify(trees_collection[pair.tree_id_1],
                                            trees_collection[pair.tree_id_2],
                                            similarity_threshold);
    if(ted_value <= similarity_threshold)
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
  using BaselineSimilarity = similarity_function::HammingBaseline;
  using LengthFilterSimilarity = similarity_function::HammingLengthFilter;
  using OptimalSimilarity = similarity_function::HammingOptimal;
  using ZhangShasha = ted::ZhangShasha<Label, CostModel>;
  using Touzet = ted::Touzet<Label, CostModel>;

  Timing timing;

  // Write results as JSON object to stdout
  std::cout << "{";
  // Path to file containing the input tree.
  std::string file_path = argv[1];

  // Set similarity threshold - maximum number of allowed edit operations.
  double similarity_threshold = std::stod(argv[2]);

  // Create the container to store all trees.
  std::vector<node::Node<Label>> trees_collection;

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

  // If algorithm is either all or allpairs baseline
  if (argv[3] == std::string("allpairs_self_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_self_join<Label, CostModel, BaselineSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_self_join<Label, CostModel, LengthFilterSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_self_join<Label, CostModel, OptimalSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      }
    } else if (argv[4] == std::string("Touzet")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_self_join<Label, CostModel, BaselineSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_self_join<Label, CostModel, LengthFilterSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_self_join<Label, CostModel, OptimalSimilarity, Touzet>(trees_collection, similarity_threshold);
      }
    }
  } 
  if (argv[3] == std::string("allpairs_multiset_baseline_self_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, BaselineSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, LengthFilterSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, OptimalSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      }
    } else if (argv[4] == std::string("Touzet")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, BaselineSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, LengthFilterSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_multiset_baseline_self_join<Label, CostModel, OptimalSimilarity, Touzet>(trees_collection, similarity_threshold);
      }
    }
  }
  if (argv[3] == std::string("allpairs_multiset_dsf_self_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, BaselineSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, LengthFilterSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, OptimalSimilarity, ZhangShasha>(trees_collection, similarity_threshold);
      }
    } else if (argv[4] == std::string("Touzet")) {
      if (argv[5] == std::string("HammingBaseline")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, BaselineSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingLengthFilter")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, LengthFilterSimilarity, Touzet>(trees_collection, similarity_threshold);
      } else if (argv[5] == std::string("HammingOptimal")) {
        execute_allpairs_multiset_dsf_self_join<Label, CostModel, OptimalSimilarity, Touzet>(trees_collection, similarity_threshold);
      }
    }
  } else if(argv[3] == std::string("naive_self_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_naive_self_join<Label, CostModel, ZhangShasha>(trees_collection, similarity_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_naive_self_join<Label, CostModel, Touzet>(trees_collection, similarity_threshold);
    }
  }

  return 0;
}