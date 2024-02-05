//
// Created by lukas on 10/6/23.
//

#include "ted_lb_experiments.h"

#include <sstream>
#include "tree_indexer.h"
#include <chrono>
#include <algorithm>
#include <random>
#include <iterator>
#include <cmath>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using Label = label::StringLabel;
using CostModelLD = cost_model::UnitCostModelLD<Label>;
using LabelDictionary = label::LabelDictionary<Label>;

using APTED = ted::APTEDTreeIndex<CostModelLD>;

using TreeCollection = std::vector<node::Node<Label>>;
using Candidates = std::vector<std::pair<int, int>>;

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}

using result_t = std::vector<std::tuple<size_t, size_t, uint>>;
using ResultPair = std::pair<std::chrono::microseconds, std::vector<std::chrono::microseconds>>;

// Trees in results can be found on their index
// meaning tree with ID=2 is on index 2, with a collection of matching trees within threshold
std::vector<std::vector<size_t>> load_results(const std::string &results_path, int threshold)
{

    std::string line;
    std::string tmp;
    std::ifstream results(results_path);
    size_t t1;
    size_t t2;
    size_t d = 0;

    std::vector<std::vector<size_t>> results_map;
    size_t result_size = 100'000;
    results_map.resize(result_size);

    while (std::getline(results, line))
    {
        auto ls = std::stringstream(line);
        std::getline(ls, tmp, ',');
        t1 = std::stoul(tmp);

        std::getline(ls, tmp, ',');
        t2 = std::stoul(tmp);

         std::getline(ls, tmp, '\n');
         d = std::stoul(tmp);

        // only load data within threshold
        if (d > threshold)
        {
            continue;
        }

        while (t1 >= results_map.size() || t2 >= results_map.size())
        {
            result_size *= 2;
            results_map.resize(result_size);
        }

        results_map[t1].emplace_back(t2);
        results_map[t2].emplace_back(t1);
    }

    for (auto & subvec: results_map) {
        std::sort(subvec.begin(), subvec.end());
    }

    return results_map;
}

ResultPair execute_sed_lb(TreeCollection & , Candidates & , int threshold, int);
ResultPair execute_sed_bounded_lb(TreeCollection & , Candidates & , int threshold, int);
ResultPair execute_label_intersection_lb(TreeCollection & , Candidates & , int threshold, int);
ResultPair execute_label_intersection_index_lb(TreeCollection & , Candidates & , int threshold);
ResultPair execute_hist_lb(TreeCollection & , Candidates & , int threshold);
ResultPair execute_bib_lb(TreeCollection & , Candidates & , int threshold);
ResultPair execute_tang_lb(TreeCollection & , Candidates & , int threshold);

void output_timepoints(std::vector<std::chrono::microseconds > sorted_timepoints, std::string & dataset_path, std::string lb_alg) {
    std::string output_file_path(dataset_path);
    auto pos = output_file_path.rfind('/');
    output_file_path.erase(output_file_path.begin() + pos, output_file_path.end());
    output_file_path.insert(pos, "/" + lb_alg + "_us.txt");

    std::cout << "Outputting times into " << output_file_path << std::endl;
    std::ofstream output (output_file_path);
    for (const auto & t: sorted_timepoints) {
        output << t.count() << "\n";
    }
    output.close();
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << "Missing arguments!\n";
        std::cerr << "Usage: \n";
        std::cerr << "\t ted-lb-experiments DATASET_PATH RESULTS_PATH [bib|sed|sed_k|lblint|lblint_ix|hist|tang] THRESHOLD\n\n";
        exit(1);
    }


    TreeCollection trees_collection;
    parser::BracketNotationParser<Label> bnp;

    std::vector<std::string> args(argv + 1, argv + argc);
    auto threshold = std::stoi(args.at(3));
    // path to dataset
    auto dataset_path = args.at(0);
    // path to precomputed correct results
    auto results_path = args.at(1);
    std::cout << "Parsing tree collection for " << dataset_path << std::endl;
    bnp.parse_collection(trees_collection, dataset_path);
    std::cout << "Parsing done" << std::endl;

    std::cout << "Loading results " << results_path << std::endl;
    auto results = load_results(results_path, threshold);
    std::cout << "Results loaded " << results_path << std::endl;

    int sample_size = 0;

    if (args.size() >= 4) {
        sample_size = std::stoi(args.at(4));
    }

    auto lb_alg = args.at(2);
    ResultPair times;
    Candidates candidates;

    if (lb_alg == "bib") {
        times = execute_bib_lb(trees_collection, candidates,  threshold);
    } else if (lb_alg == "sed") {
        times = execute_sed_lb(trees_collection, candidates, threshold, sample_size);
    } else if (lb_alg == "sed_k") {
        times = execute_sed_bounded_lb(trees_collection, candidates, threshold, sample_size);
    } else if (lb_alg == "lblint") {
        times = execute_label_intersection_lb(trees_collection, candidates, threshold, sample_size);
    } else if (lb_alg == "lblint_ix") {
        times = execute_label_intersection_index_lb(trees_collection, candidates, threshold);
    } else if (lb_alg == "hist") {
        times = execute_hist_lb(trees_collection, candidates, threshold);
    } else if (lb_alg == "tang") {
        times = execute_tang_lb(trees_collection, candidates, threshold);
    } else {
        std::cerr << "Unknown algorithm method!" << std::endl;
        exit(1);
    }

    std::sort(candidates.begin(), candidates.end());
    // check candidates against real
    auto correct = 0;
    auto incorrect = 0;
    for (const auto &pair: candidates) {
        if (std::binary_search(results[pair.first].begin(), results[pair.first].end(), pair.second)) {
            correct += 1;
        } else {
            incorrect += 1;
        }
    }
    std::cout << "Correct trees;Incorrect trees;Precision;Total Time\n";
    std::cout << correct << ";" << incorrect << ";" << 100 * ((double)correct / (double)(correct + incorrect)) << ";" << times.first.count() << "\n";
    output_timepoints(times.second, dataset_path, lb_alg);
    return 0;
}

ResultPair execute_sed_lb(TreeCollection & collection, Candidates & candidates, int threshold, int sample_size = 0) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
    std::cout << "Computing tree indexes\n";
    // preprocess - index tree collection
    std::vector<node::TreeIndexSED> tree_indexes;
    auto preprocess_start = high_resolution_clock::now();
    for (const auto & t : collection) {
        node::TreeIndexSED i;
        node::index_tree(i, t, ld, ucm);
        tree_indexes.emplace_back(i);
    }
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";

    std::vector<std::vector<size_t>> all_candidates;
    auto sed_ti = ted_lb::SEDTreeIndex<CostModelLD>(ucm);
    std::vector<std::chrono::microseconds> ted_times;
    auto total_ted_time = std::chrono::microseconds {};
    std::chrono::milliseconds total_exec_time;
    if (sample_size > 0) {
        auto n = (int)std::round((double)tree_indexes.size() * (sample_size / 100.0));
        std::vector<node::TreeIndexSED> sample_indexes;
        std::sample(tree_indexes.begin(), tree_indexes.end(), std::back_inserter(sample_indexes), n, std::mt19937 {std::random_device{}()});
        auto total_exec_time_start = high_resolution_clock ::now();
        for (int i = 0; i < sample_indexes.size(); ++i) {
            std::cout << "Running tree " << i << " of " << sample_indexes.size() << "\n";
            for (int j = i + 1; j < sample_indexes.size(); ++i) {
                auto lb = sed_ti.ted(sample_indexes[i], sample_indexes[j]);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    } else {
        auto total_exec_time_start = high_resolution_clock ::now();
        for (int i = 0; i < tree_indexes.size(); i++) {
            std::cout << "Running tree " << i << " of " << tree_indexes.size() << std::endl;
            auto ted_start = high_resolution_clock ::now();
            for (int j = i + 1; j < tree_indexes.size(); j++) {
                auto lb = sed_ti.ted(tree_indexes[i], tree_indexes[j]);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
            auto ted_time = duration_cast<std::chrono::microseconds >(high_resolution_clock::now() - ted_start);
            ted_times.emplace_back(ted_time);
            total_ted_time += ted_time;
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    }


    std::sort(ted_times.begin(), ted_times.end());

    std::cout << "Total SED execution time: " << total_exec_time.count() << "ms\n";
    std::cout << "Only  SED time: " << duration_cast<std::chrono::milliseconds >(total_ted_time).count() << "ms\n";
    return std::make_pair(total_exec_time, ted_times);
}

ResultPair execute_sed_bounded_lb(TreeCollection & collection, Candidates & candidates, int threshold, int sample_size = 0) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
    std::cout << "Computing tree indexes\n";
    // preprocess - index tree collection
    std::vector<node::TreeIndexSED> tree_indexes;
    auto preprocess_start = high_resolution_clock::now();
    for (const auto & t : collection) {
        node::TreeIndexSED i;
        node::index_tree(i, t, ld, ucm);
        tree_indexes.emplace_back(i);
    }
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";

    std::vector<std::vector<size_t>> all_candidates;
    auto sed_ti = ted_lb::SEDTreeIndex<CostModelLD>(ucm);
    std::vector<std::chrono::microseconds> ted_times;
    auto total_ted_time = std::chrono::microseconds {};
    std::chrono::milliseconds total_exec_time;
    if (sample_size > 0) {
        auto n = (int)std::round((double)tree_indexes.size() * (sample_size / 100.0));
        std::vector<node::TreeIndexSED> sample_indexes;
        std::sample(tree_indexes.begin(), tree_indexes.end(), std::back_inserter(sample_indexes), n, std::mt19937 {std::random_device{}()});
        auto total_exec_time_start = high_resolution_clock ::now();
        for (int i = 0; i < sample_indexes.size(); ++i) {
            std::cout << "Running tree " << i << " of " << sample_indexes.size() << "\n";
            for (int j = i + 1; j < sample_indexes.size(); ++i) {
                auto lb = sed_ti.ted_k(sample_indexes[i], sample_indexes[j], threshold + 1);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    } else {
        auto total_exec_time_start = high_resolution_clock ::now();
        for (int i = 0; i < tree_indexes.size(); i++) {
            std::cout << "Running tree " << i << " of " << tree_indexes.size() << std::endl;
            auto ted_start = high_resolution_clock ::now();
            for (int j = i + 1; j < tree_indexes.size(); j++) {
                auto lb = sed_ti.ted(tree_indexes[i], tree_indexes[j]);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
            auto ted_time = duration_cast<std::chrono::microseconds >(high_resolution_clock::now() - ted_start);
            ted_times.emplace_back(ted_time);
            total_ted_time += ted_time;
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    }


    std::sort(ted_times.begin(), ted_times.end());

    std::cout << "Total Bounded-SED execution time: " << total_exec_time.count() << "ms\n";
    std::cout << "Only  Bounded-SED time: " << duration_cast<std::chrono::milliseconds >(total_ted_time).count() << "ms\n";
    return std::make_pair(total_exec_time, ted_times);
}

ResultPair execute_label_intersection_lb(TreeCollection & collection, Candidates & candidates, int threshold, int sample_size = 0) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
    // preprocess - index tree collection
    std::vector<node::TreeIndexLI> tree_indexes;
    auto preprocess_start = high_resolution_clock::now();
    for (const auto & t : collection) {
        node::TreeIndexLI i;
        node::index_tree(i, t, ld, ucm);
        tree_indexes.emplace_back(i);
    }
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";
    std::vector<std::vector<size_t>> all_candidates;
    auto li_lb = ted_lb::LabelIntersection<CostModelLD, node::TreeIndexLI>(ucm);
    std::vector<std::chrono::microseconds> ted_times;
    std::chrono::microseconds total_ted_time = std::chrono::microseconds {};
    std::chrono::milliseconds total_exec_time;

    if (sample_size > 0) {
        auto n = (int)std::round((double)tree_indexes.size() * (sample_size / 100.0));
        std::vector<node::TreeIndexLI> sample_indexes;
        std::sample(tree_indexes.begin(), tree_indexes.end(), std::back_inserter(sample_indexes), n, std::mt19937 {std::random_device{}()});
        auto total_exec_time_start = high_resolution_clock ::now();
        for (int i = 0; i < sample_indexes.size(); ++i) {
            std::cout << "Running tree " << i << " of " << sample_indexes.size() << "\n";
            for (int j = i + 1; j < sample_indexes.size(); ++i) {
                auto lb = li_lb.ted(sample_indexes[i], sample_indexes[j]);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    } else {
        auto total_exec_time_start = high_resolution_clock ::now();

        for (int i = 0; i < tree_indexes.size(); i++) {
            std::cout << "Processing tree " << i << " of " << tree_indexes.size() << "\n";
            auto ted_start = high_resolution_clock ::now();
            for (int j = i + 1; j < tree_indexes.size(); j++) {
                auto lb = li_lb.ted(tree_indexes[i], tree_indexes[j]);
                if (lb <= threshold) {
                    candidates.emplace_back(std::make_pair(i, j));
                }
            }
            auto ted_time = duration_cast<std::chrono::microseconds >(high_resolution_clock::now() - ted_start);
            ted_times.emplace_back(ted_time);
            total_ted_time += ted_time;
        }
        total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    }


    std::sort(ted_times.begin(), ted_times.end());

    std::cout << "Total LI execution time: " << total_exec_time.count() << "ms\n";
    std::cout << "Only  LI time: " << duration_cast<std::chrono::milliseconds >(total_ted_time).count() << "ms\n";
    return std::make_pair(total_exec_time, ted_times);
}

ResultPair execute_label_intersection_index_lb(TreeCollection & collection, Candidates & candidates, int threshold) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
    std::vector<std::pair<int, std::vector<label_set_converter::LabelSetElement>>> sets_collection;
    // preprocess - index tree collection
    std::vector<node::TreeIndexLI> tree_indexes;
    join::TJoinTI<Label, APTED > alg;
    auto preprocess_start = high_resolution_clock::now();
    alg.convert_trees_to_sets(collection, sets_collection);
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";

    std::vector<std::vector<size_t>> all_candidates;
    auto li_lb = ted_lb::LabelIntersection<CostModelLD, node::TreeIndexLI>(ucm);

    std::vector<std::chrono::microseconds> ted_times;

    auto total_exec_time_start = high_resolution_clock ::now();
    // Retrieves candidates from the candidate index.
    alg.retrieve_candidates(sets_collection, candidates, threshold, ted_times);
    auto total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);

    std::sort(ted_times.begin(), ted_times.end());
    std::cout << "Total IndexLI execution time: " << total_exec_time.count() << "ms\n";
//    std::cout << "Only  IndexLI time: " << duration_cast<std::chrono::milliseconds >(total_ted_time).count() << "ms\n";
    return std::make_pair(total_exec_time, ted_times);
}

ResultPair execute_hist_lb(TreeCollection & collection, Candidates & candidates, int threshold) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
    std::vector<std::pair<int, std::unordered_map<int, int>>> label_histogram_collection;
    std::vector<std::pair<int, std::unordered_map<int, int>>> degree_histogram_collection;
    std::vector<std::pair<int, std::unordered_map<int, int>>> leaf_distance_histogram_collection;
    std::vector<std::chrono::microseconds> ted_times;
    // convert trees into histograms
    histogram_converter::Converter<Label> hc;
    auto preprocess_start = high_resolution_clock::now();
    hc.create_histogram(collection, label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection);
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";
    // Initialize candidate index.
    histo_candidate_index::CandidateIndex c_index;

    auto il_size_ = hc.get_number_of_labels();

    auto total_exec_time_start = high_resolution_clock ::now();
    // Retrieve candidates from the candidate index.
    c_index.lookup(
            label_histogram_collection, degree_histogram_collection, leaf_distance_histogram_collection,candidates, il_size_, threshold, ted_times
            );
    auto total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    std::cout << "Total HIST execution time: " << total_exec_time.count() << "ms\n";
    std::sort(ted_times.begin(), ted_times.end());
    return std::make_pair(total_exec_time, ted_times);
}

ResultPair execute_bib_lb(TreeCollection & collection, Candidates & candidates, int threshold) {
    std::vector<std::pair<int, std::unordered_map<int, int>>> histogram_collection;
    bin_branch_histogram_converter::Converter<Label> bbhc;
    auto preprocess_start = high_resolution_clock::now();
    bbhc.create_histogram(collection, histogram_collection);
    auto il_size_ = bbhc.get_number_of_bb();
    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";

    // Initialize candidate index.
    bb_candidate_index::CandidateIndex c_index;
    std::vector<std::chrono::microseconds> ted_times;
    auto total_exec_time_start = high_resolution_clock ::now();
    // Retrieve candidates from the candidate index.
    c_index.lookup(histogram_collection, candidates, il_size_, threshold, ted_times);
    auto total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    std::cout << "Total BIB execution time: " << total_exec_time.count() << "ms\n";
    std::sort(ted_times.begin(), ted_times.end());
    return std::make_pair(total_exec_time, ted_times);
}


ResultPair execute_tang_lb(TreeCollection & collection, Candidates & candidates, int threshold) {
    std::vector<node::BinaryNode<Label>> binary_trees_collection;
    std::unordered_set<std::pair<int, int>, join::hashintegerpair> ix_candidates;
    auto preprocess_start = high_resolution_clock::now();
    join::TangJoinTI<Label, APTED> alg;
    alg.convert_trees_to_binary_trees(collection, binary_trees_collection);

    auto preprocess_stop = high_resolution_clock ::now();
    auto preprocessing = duration_cast<std::chrono::milliseconds >(preprocess_stop  - preprocess_start);
    std::cout << "Preprocessing time: " << preprocessing.count() << "ms\n";

    // Initialize candidate index.
    std::vector<std::chrono::microseconds> ted_times;
    auto total_exec_time_start = high_resolution_clock ::now();
    // Retrieve candidates from the candidate index.
    alg.retrieve_candidates(binary_trees_collection, ix_candidates, threshold, ted_times);
    auto total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);
    std::cout << "Total TANG execution time: " << total_exec_time.count() << "ms\n";

    for(const auto& pair: ix_candidates) {
        candidates.emplace_back(pair);
    }
    std::sort(ted_times.begin(), ted_times.end());
    return std::make_pair(total_exec_time, ted_times);
}
