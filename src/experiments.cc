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

int main(int argc, char** argv) {

  if(argc != 4) {
    std::cout << "Usage: ./experiments <input_file> <similarity_threshold> <algorithm>"
        << std::endl;
    std::cout << "Algorithm:" << std::endl;
    std::cout << "all ... all algorithms" << std::endl << "allpairs_baseline ... allpairs baseline" << std::endl <<
        "naive ... naive join" << std::endl;
    exit(-1);
  }


  using Label = label::StringLabel;
  using CostModel = cost_model::UnitCostModel<Label>;
  using BaselineSimilarity = similarity_function::HammingBaseline;
  using LengthFilterSimilarity = similarity_function::HammingLengthFilter;
  using OptimalSimilarity = similarity_function::HammingOptimal;
  Timing timing;


  // Write results as JSON object to stdout
  std::cout << "{";
  // Path to file containing the input tree.
  std::string file_path = argv[1];
  // std::cout << "\"input_file\": \"" << file_path << "\", ";
  // Set similarity threshold - maximum number of allowed edit operations.
  double similarity_threshold = std::stod(argv[2]);
  // std::cout << "\"threshold\": " << similarity_threshold << ", ";

  // Create the container to store all trees.
  std::vector<node::Node<Label>> trees_collection;

  ////////////////////////////////////////////////////////////////////////
  /// PARSE INPUT
  ////////////////////////////////////////////////////////////////////////

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


  ////////////////////////////////////////////////////////////////////////
  /// ALLPAIRS BASELINE ALGORITHM
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("all") || argv[3] == std::string("allpairs_baseline")) {
    // Initialize allpairs baseline
    join::AllpairsGenericSelfJoin<Label, CostModel, BaselineSimilarity> absj;

    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::vector<unsigned int>> sets_collection =
        absj.convert_trees_to_sets(trees_collection);

    // Stop timing
    tree_to_set->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * allpairs = timing.create_enroll("Allpairs");
    // Start timing
    allpairs->start();

    // Compute candidate for the join with the allpairs algorithm
    std::vector<std::pair<unsigned int, unsigned int>> join_candidates_absj =
        absj.get_join_candidates(sets_collection, similarity_threshold);

    // Stop timing
    allpairs->stop();

    // Write timing
    std::cout << "\"filter_time\": " << allpairs->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * verify = timing.create_enroll("Verify");
    // Start timing
    verify->start();

    // Verify all computed join candidates and return the join result
    std::vector<join::JoinResultElement> result_set_absj =
        absj.verify_candidates(trees_collection, join_candidates_absj,
                               similarity_threshold);

    // Stop timing
    verify->stop();

    // Write timing
    std::cout << "\"verification_time\": " << verify->getfloat() << ", ";


    // Write number of candidates and number of result pairs
    std::cout << "\"verification_candidates\": " << join_candidates_absj.size() << ", ";
    std::cout << "\"result_set_size\": " << result_set_absj.size() << "}" << std::endl;
  }


  ////////////////////////////////////////////////////////////////////////
  /// ALLPAIRS LENGTH FILTER ALGORITHM
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("all") || argv[3] == std::string("allpairs_length_filter")) {
    // Initialize allpairs baseline
    join::AllpairsGenericSelfJoin<Label, CostModel, LengthFilterSimilarity> alfsj;

    // Initialized Timing object
    Timing::Interval * tree_to_set_lf = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set_lf->start();

    // Convert trees to sets and get the result.
    std::vector<std::vector<unsigned int>> sets_collection =
        alfsj.convert_trees_to_sets(trees_collection);

    // Stop timing
    tree_to_set_lf->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set_lf->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * allpairs_lf = timing.create_enroll("Allpairs");
    // Start timing
    allpairs_lf->start();

    // Compute candidate for the join with the allpairs algorithm
    std::vector<std::pair<unsigned int, unsigned int>> join_candidates_alfsj =
        alfsj.get_join_candidates(sets_collection, similarity_threshold);

    // Stop timing
    allpairs_lf->stop();

    // Write timing
    std::cout << "\"filter_time\": " << allpairs_lf->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * verify_lf = timing.create_enroll("Verify");
    // Start timing
    verify_lf->start();

    // Verify all computed join candidates and return the join result
    std::vector<join::JoinResultElement> result_set_alfsj =
        alfsj.verify_candidates(trees_collection, join_candidates_alfsj,
                               similarity_threshold);

    // Stop timing
    verify_lf->stop();

    // Write timing
    std::cout << "\"verification_time\": " << verify_lf->getfloat() << ", ";


    // Write number of candidates and number of result pairs
    std::cout << "\"verification_candidates\": " << join_candidates_alfsj.size() << ", ";
    std::cout << "\"result_set_size\": " << result_set_alfsj.size() << "}" << std::endl;
  }


  ////////////////////////////////////////////////////////////////////////
  /// ALLPAIRS OPTIMAL ALGORITHM
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("all") || argv[3] == std::string("allpairs_optimal")) {
    // Initialize allpairs baseline
    join::AllpairsGenericSelfJoin<Label, CostModel, OptimalSimilarity> aosj;

    // Initialized Timing object
    Timing::Interval * tree_to_set_o = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set_o->start();

    // Convert trees to sets and get the result.
    std::vector<std::vector<unsigned int>> sets_collection =
        aosj.convert_trees_to_sets(trees_collection);

    // Stop timing
    tree_to_set_o->stop();

    // Write timing
    std::cout << "\"tree_to_set_time\": " << tree_to_set_o->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * allpairs_o = timing.create_enroll("Allpairs");
    // Start timing
    allpairs_o->start();

    // Compute candidate for the join with the allpairs algorithm
    std::vector<std::pair<unsigned int, unsigned int>> join_candidates_aosj =
        aosj.get_join_candidates(sets_collection, similarity_threshold);

    // Stop timing
    allpairs_o->stop();

    // Write timing
    std::cout << "\"filter_time\": " << allpairs_o->getfloat() << ", ";


    // Initialized Timing object
    Timing::Interval * verify_o = timing.create_enroll("Verify");
    // Start timing
    verify_o->start();

    // Verify all computed join candidates and return the join result
    std::vector<join::JoinResultElement> result_set_aosj =
        aosj.verify_candidates(trees_collection, join_candidates_aosj,
                               similarity_threshold);

    // Stop timing
    verify_o->stop();

    // Write timing
    std::cout << "\"verification_time\": " << verify_o->getfloat() << ", ";


    // Write number of candidates and number of result pairs
    std::cout << "\"verification_candidates\": " << join_candidates_aosj.size() << ", ";
    std::cout << "\"result_set_size\": " << result_set_aosj.size() << "}" << std::endl;
  }


  ////////////////////////////////////////////////////////////////////////
  /// NAIVE SELF JOIN
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("all") || argv[3] == std::string("naive")) {
    // Initialize allpairs baseline
    join::NaiveSelfJoin<Label, CostModel> nsj;

    // Initialized Timing object
    Timing::Interval * naive_join = timing.create_enroll("NaiveJoin");
    // Start timing
    naive_join->start();

    // Verify all computed join candidates and return the join result
    std::vector<join::JoinResultElement> result_set_nsj =
        nsj.execute_join(trees_collection, similarity_threshold);

    // Stop timing
    naive_join->stop();

    // Write timing and number of result pairs
    int num_trees = trees_collection.size();
    std::cout << "\"verification_candidates\" : " << (num_trees*num_trees-num_trees)/2 << ", ";
    std::cout << "\"verification_time\" : " << naive_join->getfloat() << ", ";
    std::cout << "\"result_set_size\" : " << result_set_nsj.size() << "}" << std::endl;
  }
}
