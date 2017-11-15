#include "experiments.h"

int main(int argc, char** argv) {

  if(argc != 4) {
    std::cout << "Uasge: ./experiments <input_file> <similarity_threshold> <algorithm>" 
        << std::endl;
    std::cout << "Algorithm:" << std::endl;
    std::cout << "a ... all algorithms" << std::endl << "b ... allpairs baseline" << std::endl <<
        "n ... naive join" << std::endl;
    exit(-1);
  }


  using Label = label::StringLabel;
  using CostModel = cost_model::UnitCostModel<Label>;
  Timing timing;

  // Path to file containing the input tree.
  std::string file_path = argv[1];
  // Set similarity threshold - maximum number of allowed edit operations.
  double similarity_threshold = std::stod(argv[2]);

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


  ////////////////////////////////////////////////////////////////////////
  /// ALLPAIRS BASELINE ALGORITHM
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("a") || argv[3] == std::string("b")) {
    // Initialize allpairs baseline
    join::AllpairsBaselineSelfJoin<Label, CostModel> absj;
  
    // Initialized Timing object
    Timing::Interval * tree_to_set = timing.create_enroll("TreeToSet");
    // Start timing
    tree_to_set->start();

    // Convert trees to sets and get the result.
    std::vector<std::vector<unsigned int>> sets_collection = 
        absj.convert_trees_to_sets(trees_collection);

    // Stop timing
    tree_to_set->stop();


    // Initialized Timing object
    Timing::Interval * allpairs = timing.create_enroll("Allpairs");
    // Start timing
    allpairs->start();

    // Compute candidate for the join with the allpairs algorithm
    std::vector<std::pair<unsigned int, unsigned int>> join_candidates_absj = 
        absj.get_join_candidates(sets_collection, similarity_threshold);

    // Stop timing
    allpairs->stop();

    std::cout << "#ABSJ: candidates=" << join_candidates_absj.size() << std::endl;

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

    std::cout << "ABSJ: #resultset=" << result_set_absj.size() << std::endl;
  }

  ////////////////////////////////////////////////////////////////////////
  /// NAIVE SELF JOIN
  ////////////////////////////////////////////////////////////////////////

  // If algorithm is either all or allpairs baseline
  if(argv[3] == std::string("a") || argv[3] == std::string("n")) {
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

    std::cout << "NSJ: #resultset=" << result_set_nsj.size() << std::endl;
  }

  // Output timing statistics
  std::cout << timing;
}
