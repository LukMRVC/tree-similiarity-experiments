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
void execute_bb_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::BBJoin<Label, CostModel, VerificationAlgorithm> bbj;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to histogram of binary branches.
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> histogram_collection;
    bbj.convert_trees_to_sets(trees_collection, histogram_collection);

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
      bbj.retrieve_candidates(histogram_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::vector<std::pair<unsigned int, unsigned int>>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);
          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            *it = join_candidates.back();
            join_candidates.pop_back();
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
      bbj.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << bbj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << bbj.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << bbj.get_subproblem_count() << ", ";
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
void execute_dh_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::DHJoin<Label, CostModel, VerificationAlgorithm> dh;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to histograms of degree values.
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> histogram_collection;
    dh.convert_trees_to_sets(trees_collection, histogram_collection);

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
      dh.retrieve_candidates(histogram_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::vector<std::pair<unsigned int, unsigned int>>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);
          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            *it = join_candidates.back();
            join_candidates.pop_back();
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
      dh.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << dh.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << dh.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << dh.get_subproblem_count() << ", ";
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
void execute_ldh_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::LDHJoin<Label, CostModel, VerificationAlgorithm> ldh;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to histograms of leaf distance values.
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> histogram_collection;
    ldh.convert_trees_to_sets(trees_collection, histogram_collection);

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
      ldh.retrieve_candidates(histogram_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::vector<std::pair<unsigned int, unsigned int>>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);
          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            *it = join_candidates.back();
            join_candidates.pop_back();
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
      ldh.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << ldh.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << ldh.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << ldh.get_subproblem_count() << ", ";
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
void execute_lh_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::LHJoin<Label, CostModel, VerificationAlgorithm> lh;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to histogram of label values.
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> histogram_collection;
    lh.convert_trees_to_sets(trees_collection, histogram_collection);

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
      lh.retrieve_candidates(histogram_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::vector<std::pair<unsigned int, unsigned int>>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);
          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            *it = join_candidates.back();
            join_candidates.pop_back();
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
      lh.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << lh.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << lh.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << lh.get_subproblem_count() << ", ";
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
void execute_histogram_join(std::vector<node::Node<Label>>& trees_collection, std::string upperbound, 
    double distance_threshold) {
  // Initialize join algorithm
  join::HJoin<Label, CostModel, VerificationAlgorithm> hj;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to histogram of label values.
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> label_histogram_collection;
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> degree_histogram_collection;
    std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>> leaf_distance_histogram_collection;
    hj.convert_trees_to_sets(trees_collection, label_histogram_collection, 
        degree_histogram_collection, leaf_distance_histogram_collection);

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
      hj.retrieve_candidates(label_histogram_collection, degree_histogram_collection, 
        leaf_distance_histogram_collection, join_candidates, distance_threshold);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"index_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"verification_candidates\": " << join_candidates.size() << ", ";

      if(upperbound == "greedy") {
        ted_ub::GreedyUB<Label, CostModel> gub;

        // Initialized Timing object
        Timing::Interval * greedyub = timing.create_enroll("GreedyUB");
        // Start timing
        greedyub->start();

        std::vector<std::pair<unsigned int, unsigned int>>::iterator it = join_candidates.begin();
        while(it != join_candidates.end()) {
          double ub_value = gub.verify(trees_collection[it->first],
                                        trees_collection[it->second],
                                        distance_threshold);
          if(ub_value <= distance_threshold) {
            join_result.emplace_back(it->first, it->second, ub_value);
            *it = join_candidates.back();
            join_candidates.pop_back();
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
      hj.verify_candidates(trees_collection, join_candidates,
                             join_result, distance_threshold);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    // Write number of candidates and number of result pairs
    std::cout << "\"index_verification_candidates\": " << hj.get_number_of_pre_candidates() << ", ";
    std::cout << "\"inv_list_lookups\": " << hj.get_number_of_il_lookups() << ", ";
    std::cout << "\"sum_subproblems\": " << hj.get_subproblem_count() << ", ";
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

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_guha_join(std::vector<node::Node<Label>>& trees_collection,
    double distance_threshold, unsigned int reference_set_size) {
  // Initialize join algorithm
  join::Guha<Label, CostModel, VerificationAlgorithm> guha_join;
  Timing timing;
  std::vector<join::JoinResultElement> join_result;

  // Add some scopes to ensure that the memory is deallocated
  {
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("Vectors");
    // Start timing
    tree_to_set->start();

    // Get a random reference set.
    std::vector<unsigned int> reference_set = guha_join.get_random_reference_set(
        trees_collection, reference_set_size
    );
    // Initialize vectors.
    std::vector<std::vector<double>> ted_vectors(trees_collection.size(), std::vector<double>(reference_set.size()));
    // Compute the vectors.
    guha_join.compute_vectors(trees_collection, reference_set, ted_vectors);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"vectors_time\": " << tree_to_set->getfloat() << ", ";

    {
      // Initialized Timing object
      Timing::Interval * retCand = timing.create_enroll("RetrieveCandidates");
      // Start timing
      retCand->start();

      // Retrieve candidates for tjoin's candidate index
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      guha_join.retrieve_candidates(trees_collection, join_candidates, join_result, distance_threshold, reference_set, ted_vectors);

      // Stop timing
      retCand->stop();

      // Write timing
      std::cout << "\"candidates_time\": " << retCand->getfloat() << ", ";
      std::cout << "\"ted_verification_candidates\": " << join_candidates.size() << ", ";
      std::cout << "\"l_t_candidates\": " << guha_join.get_l_t_candidates() << ", ";
      std::cout << "\"sed_candidates\": " << guha_join.get_sed_candidates() << ", ";
      std::cout << "\"u_t_result_pairs\": " << guha_join.get_u_t_result_pairs() << ", ";
      std::cout << "\"cted_result_pairs\": " << guha_join.get_cted_result_pairs() << ", ";

      // Initialized Timing object
      Timing::Interval * verify = timing.create_enroll("Verify");
      // Start timing
      verify->start();

      // Verify all computed join candidates and return the join result
      guha_join.verify_candidates(trees_collection, join_candidates, join_result, distance_threshold, ted_vectors);

      // Stop timing
      verify->stop();

      // Write timing
      std::cout << "\"verification_time\": " << verify->getfloat() << ", ";
    }
    std::cout << "\"join_result_size\": " << join_result.size() << "}" << std::endl;
  }

  // Calculate optimum by verify only the resultset
  // Initialized Timing object
  Timing::Interval * optimum = timing.create_enroll("Optimum");
  // Start timing
  optimum->start();
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
  } else if(argv[3] == std::string("leaf_distance_histogram_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_ldh_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_ldh_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_ldh_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if(argv[3] == std::string("label_histogram_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_lh_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_lh_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_lh_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if(argv[3] == std::string("degree_histogram_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_dh_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_dh_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_dh_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if(argv[3] == std::string("binary_branches_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_bb_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_bb_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_bb_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if(argv[3] == std::string("histogram_join")) {
    if (argv[4] == std::string("ZhangShasha")) {
      execute_histogram_join<Label, CostModel, ZhangShasha>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("Touzet")) {
      execute_histogram_join<Label, CostModel, Touzet>(trees_collection, upperbound, distance_threshold);
    } else if (argv[4] == std::string("APTED")) {
      execute_histogram_join<Label, CostModel, APTED>(trees_collection, upperbound, distance_threshold);
    }
  } else if (argv[3] == std::string("guha_join")) {
    unsigned int reference_set_size = std::stoi(argv[7]);
    if (argv[4] == std::string("ZhangShasha")) {
      execute_guha_join<Label, CostModel, ZhangShasha>(trees_collection, distance_threshold, reference_set_size);
    } else if (argv[4] == std::string("Touzet")) {
      execute_guha_join<Label, CostModel, Touzet>(trees_collection, distance_threshold, reference_set_size);
    } else if (argv[4] == std::string("APTED")) {
      execute_guha_join<Label, CostModel, APTED>(trees_collection, distance_threshold, reference_set_size);
    }
  }

  return 0;
}
