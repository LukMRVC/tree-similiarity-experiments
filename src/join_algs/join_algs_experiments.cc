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
void execute_guha_rsb_join(std::vector<node::Node<Label>>& trees_collection,
    double distance_threshold, unsigned int reference_set_size, int reference_set_id) {
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

    // In case there is no reference set, get a random reference set.
    std::vector<unsigned int> reference_set;
    if (reference_set_id == -1) {
      reference_set = guha_join.get_random_reference_set(trees_collection, reference_set_size);
    } else {
      auto first = reference_sets_[reference_set_id].begin();
      auto last = reference_sets_[reference_set_id].begin() + reference_set_size;
      reference_set = std::vector<unsigned int>(first, last);
    }
    
    // Initialize vectors.
    std::vector<std::vector<double>> ted_vectors(trees_collection.size(), std::vector<double>(reference_set.size()));
    // Compute the vectors.
    guha_join.compute_vectors(trees_collection, reference_set, ted_vectors);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"vectors_time\": " << tree_to_set->getfloat() << ", ";

    {
      Timing::Interval * retMetricCand = timing.create_enroll("RetrieveMetricCandidates");
      retMetricCand->start();
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      guha_join.retrieve_metric_candidates(trees_collection, join_candidates, join_result, distance_threshold, ted_vectors);
      retMetricCand->stop();
      
      Timing::Interval * retSCCand = timing.create_enroll("RetrieveSCCandidates");
      retSCCand->start();
      guha_join.retrieve_sc_candidates(trees_collection, join_candidates, join_result, distance_threshold);
      retSCCand->stop();

      // Write timing
      std::cout << "\"metric_candidates_time\": " << retMetricCand->getfloat() << ", ";
      std::cout << "\"sc_candidates_time\": " << retSCCand->getfloat() << ", ";
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
      guha_join.verify_candidates(trees_collection, join_candidates, join_result, distance_threshold);

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

template <typename Label, typename CostModel, typename VerificationAlgorithm>
void execute_guha_rsc_join(std::vector<node::Node<Label>>& trees_collection,
    double distance_threshold, unsigned int reference_set_size, int reference_set_id) {
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

    // In case there is no reference set, get a random reference set.
    std::vector<unsigned int> reference_set;
    if (reference_set_id == -1) {
      reference_set = guha_join.get_random_reference_set(trees_collection, reference_set_size);
    } else {
      auto first = reference_sets_[reference_set_id].begin();
      auto last = reference_sets_[reference_set_id].begin() + reference_set_size;
      reference_set = std::vector<unsigned int>(first, last);
    }
    
    // Initialize vectors.
    std::vector<std::vector<double>> lb_vectors(trees_collection.size(), std::vector<double>(reference_set.size()));
    std::vector<std::vector<double>> ub_vectors(trees_collection.size(), std::vector<double>(reference_set.size()));
    // Compute the vectors.
    guha_join.compute_vectors(trees_collection, reference_set, lb_vectors, ub_vectors);
    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"vectors_time\": " << tree_to_set->getfloat() << ", ";

    {
      Timing::Interval * retMetricCand = timing.create_enroll("RetrieveMetricCandidates");
      retMetricCand->start();
      std::vector<std::pair<unsigned int, unsigned int>> join_candidates;
      guha_join.retrieve_metric_candidates(trees_collection, join_candidates, join_result, distance_threshold, lb_vectors, ub_vectors);
      retMetricCand->stop();
      
      Timing::Interval * retSCCand = timing.create_enroll("RetrieveSCCandidates");
      retSCCand->start();
      guha_join.retrieve_sc_candidates(trees_collection, join_candidates, join_result, distance_threshold);
      retSCCand->stop();

      // Write timing
      std::cout << "\"metric_candidates_time\": " << retMetricCand->getfloat() << ", ";
      std::cout << "\"sc_candidates_time\": " << retSCCand->getfloat() << ", ";
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
      guha_join.verify_candidates(trees_collection, join_candidates, join_result, distance_threshold);

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

  // [0] Sentiment tau = 1: (manually added 7661, 9501, 1398, 292, 2856, 7230)
  reference_sets_.push_back({18,11,2,33,20,293,91,65,6426,5113,1338,1257,317,258,254,252,237,229,137,111,86,81,44,41,28,14,7661,9501,1398,292,2856,7230});
  // [1] Sentiment tau = 2:
  reference_sets_.push_back({12,57,105,96,74,92,49,284,127,37,302,158,136,133,86,34,296,293,173,156,138,53,6426,5113,1338,1257,462,447,431,358,318,299,285,245,278,272,259,254,251,223,236,228,221,203,164,144,128,119,117,114,112,81,51});
  // [2] Sentiment tau = 3:
  reference_sets_.push_back({19,55,80,295,286,93,49,123,161,175,220,224,156,146,272,194,260,423,321,293,268,244,227,221,204,200,108,86,6426,5113,1338,1258,763,786,671,373,443,470,466,462,446,440,430,419,405,393,329,359,354,327,317,259,251,237,232,214,210,206,181,180,56});
  // [3] Sentiment tau = 4:
  reference_sets_.push_back({40,228,177,296,251,236,194,155,371,43,669,282,384,723,459,624,358,446,134,323,691,329,474,402,609,317,328,235,212,213,260,6426,5113,1338,1258,763,786,737,728,716,680,672,601,546,438,468,448,439,431,412,330,362,349,315,326,311,307,290,207,69,1232,722,599,511,475,461});
  // [4] Sentiment tau = 5:
  reference_sets_.push_back({40,144,290,354,346,377,565,362,458,438,467,225,723,452,550,457,398,695,672,329,602,510,426,390,393,311,163,871,739,769,730,686,615,359,324,416,406,410,402,330,340,535,6426,5113,1841,809,1337,1257,1163,763,722,940,842,786,479,724,705,598,620,569,546,537,464,421,322,412,401,369,364,1204,948,731,706,699,675,555,529,492,462});
  // [5] Sentiment tau = 6:
  reference_sets_.push_back({81,424,546,720,510,624,592,311,907,529,728,640,344,770,472,938,725,454,396,714,500,970,486,568,549,305,867,689,673,1258,871,974,943,810,480,693,667,427,575,453,648,483,6425,5113,1837,1458,1360,1337,1322,1227,1185,1046,1037,885,918,864,855,816,471,705,699,637,629,591,559,524,496,1510,1204,844,1099,710,948,917,784,764,704,675,603,585});
  // [6] Sentiment tau = 7:
  reference_sets_.push_back({81,462,677,348,622,659,733,801,520,911,483,478,649,772,616,504,820,487,1905,756,809,946,938,915,1511,502,932,866,707,607,1837,1111,1350,1227,952,1103,670,815,791,703,528,482,1187,641,6426,5113,1778,1458,1337,1054,1232,1176,1193,1191,1161,851,1030,763,1022,896,943,888,686,786,788,577,628,571,526,503,1621,1523,957,825,936,916,917,856,840,813,807,518,784,765,742,652,618,613,603,589});


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
  } else if (argv[3] == std::string("guha_rsb_join")) {
    unsigned int reference_set_size = std::stoi(argv[7]);
    int reference_set_id = std::stoi(argv[8]);
    if (argv[4] == std::string("ZhangShasha")) {
      execute_guha_rsb_join<Label, CostModel, ZhangShasha>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    } else if (argv[4] == std::string("Touzet")) {
      execute_guha_rsb_join<Label, CostModel, Touzet>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    } else if (argv[4] == std::string("APTED")) {
      execute_guha_rsb_join<Label, CostModel, APTED>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    }
  } else if (argv[3] == std::string("guha_rsc_join")) {
    unsigned int reference_set_size = std::stoi(argv[7]);
    int reference_set_id = std::stoi(argv[8]);
    if (argv[4] == std::string("ZhangShasha")) {
      execute_guha_rsc_join<Label, CostModel, ZhangShasha>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    } else if (argv[4] == std::string("Touzet")) {
      execute_guha_rsc_join<Label, CostModel, Touzet>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    } else if (argv[4] == std::string("APTED")) {
      execute_guha_rsc_join<Label, CostModel, APTED>(trees_collection, distance_threshold, reference_set_size, reference_set_id);
    }
  }

  return 0;
}
