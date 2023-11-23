//
// Created by lukas on 10/6/23.
//

#include "ted_lb_experiments.h"

#include "histo_join_ti.h"
#include "bb_join_ti.h"
#include "t_join_ti.h"
#include <sstream>
#include "tree_indexer.h"
#include <chrono>



using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using Label = label::StringLabel;
using CostModelLD = cost_model::UnitCostModelLD<Label>;
using LabelDictionary = label::LabelDictionary<Label>;

using TreeCollection = std::vector<node::Node<Label>>;

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

        while (t1 >= results_map.size() ||t2 >= results_map.size())
        {
            result_size *= 2;
            results_map.resize(result_size);
        }

        results_map[t1].emplace_back(t2);
        results_map[t2].emplace_back(t1);
    }

    return results_map;
}

std::vector<std::chrono::microseconds> execute_sed_lb(TreeCollection & , int threshold);

void output_timepoints(std::vector<std::chrono::microseconds > sorted_timepoints, std::string & dataset_path) {
    std::string output_file_path(dataset_path);
    auto pos = output_file_path.rfind('/');
    output_file_path.erase(output_file_path.begin() + pos, output_file_path.end());
    output_file_path.insert(pos, "/ted-times.txt");
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
        std::cerr << "\t ted-lb-experiments DATASET_PATH RESULTS_PATH [bib|sed|lblint|leafhist|deghist|lblhist] THRESHOLD\n\n";
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

    std::cout << "Loading results " << dataset_path << std::endl;
    auto results = load_results(results_path, threshold);

    auto lb_alg = args.at(2);
    std::vector<std::chrono::microseconds> times;

    if (lb_alg == "bib") {
        perror("Not implemented");
        exit(1);
    } else if (lb_alg == "sed") {
        times = execute_sed_lb(trees_collection, threshold);
    } else if (lb_alg == "lblint") {
        perror("Not implemented");
        exit(1);
    } else if (lb_alg == "leafhist") {
        perror("Not implemented");
        exit(1);
    } else if (lb_alg == "deghist") {
        perror("Not implemented");
        exit(1);
    } else if (lb_alg == "lblhist") {
        perror("Not implemented");
        exit(1);
    }

    output_timepoints(times, dataset_path);
    return 0;
}

std::vector<std::chrono::microseconds> execute_sed_lb(TreeCollection & collection, int threshold) {
    LabelDictionary ld;
    CostModelLD ucm(ld);
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

    auto total_exec_time_start = high_resolution_clock ::now();
    auto total_ted_time = std::chrono::microseconds {};
    for (int i = 0; i < tree_indexes.size(); i++) {
        std::vector<size_t> candidates;
        for (int j = i + 1; j < tree_indexes.size(); j++) {
            auto ted_start = high_resolution_clock ::now();
            auto lb = sed_ti.ted(tree_indexes[i], tree_indexes[j]);
            auto ted_time = duration_cast<std::chrono::microseconds >(high_resolution_clock::now() - ted_start);
            ted_times.emplace_back(ted_time);
            total_ted_time += ted_time;
            if (lb <= threshold) {
                candidates.emplace_back(j);
            }
        }
        all_candidates.emplace_back(candidates);
    }
    auto total_exec_time = duration_cast<std::chrono::milliseconds>(high_resolution_clock::now() - total_exec_time_start);

    std::sort(ted_times.begin(), ted_times.end());

    std::cout << "Total SED execution time: " << total_exec_time.count() << "ms\n";
    std::cout << "Only  SED time: " << duration_cast<std::chrono::milliseconds >(total_ted_time).count() << "ms\n";
    return ted_times;
}