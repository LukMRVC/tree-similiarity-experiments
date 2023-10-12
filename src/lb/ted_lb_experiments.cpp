//
// Created by lukas on 10/6/23.
//

#include "ted_lb_experiments.h"

#include "histo_join.h"
#include "bb_join.h"
#include "t_join.h"

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

using result_t = std::vector<std::tuple<size_t, size_t, double>>;

// histogram collection type
using histo_col_t = std::vector<std::pair<unsigned int, std::unordered_map<unsigned int, unsigned int>>>;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Missing arguments for dataset path, thresholds and filter type [histo|bib|struct]!\n";
        exit(1);
    }
    std::vector<std::string> args(argv, argv + argc);
    auto dataset_path = args.at(1);
    using Label = label::StringLabel;
    using CostModel = cost_model::UnitCostModel<Label>;
    using APTED = ted::APTED<Label, CostModel>;

    std::vector<node::Node<Label>> trees_collection;
    parser::BracketNotationParser bnp;
    std::cout << "Parsing tree collection for " << dataset_path << std::endl;
    bnp.parse_collection(trees_collection, dataset_path);
    std::cout << "Parsing done" << std::endl;

    histo_col_t label_hists;
    histo_col_t left_dist_hists;
    histo_col_t degree_hists;

    const auto str_thresholds = split(args.at(2), ',');
    std::vector<double> thresholds;
    for (auto t : str_thresholds)
    {
        thresholds.emplace_back(std::stod(t));
    }

    std::vector<std::pair<unsigned int, unsigned int>> candidates;

    auto joinType = args.at(3);

    if (joinType == "histo")
    {
        join::HJoin<Label, CostModel, APTED> histo_join;
        // convert the tree collection into histograms to then execute filters
        histo_join.convert_trees_to_histograms(trees_collection, label_hists, degree_hists, left_dist_hists);
        for (const auto threshold : thresholds)
        {
            std::cout << "--- Threshold " << threshold << " candidates\n";
            histo_join.retrieve_candidates(label_hists, degree_hists, left_dist_hists, candidates, threshold);
            for (const auto &candidate : candidates)
            {
                auto t1 = std::get<0>(candidate);
                auto t2 = std::get<1>(candidate);
                std::cout << t1 << " -> " << t2 << "\n";
            }
        }
    }
    else if (joinType == "bib")
    {
        join::BBJoin<Label, CostModel, APTED> bib_join;
        bib_join.convert_trees_to_histograms(trees_collection, label_hists);
        for (const auto threshold : thresholds)
        {
            std::cout << "--- Threshold " << threshold << " candidates\n";
            bib_join.retrieve_candidates(label_hists, candidates, threshold);
            for (const auto &candidate : candidates)
            {
                auto t1 = std::get<0>(candidate);
                auto t2 = std::get<1>(candidate);
                std::cout << t1 << " -> " << t2 << "\n";
            }
        }
    }
    else if (joinType == "struct")
    {
        std::vector<std::pair<unsigned int, std::vector<label_set_converter::LabelSetElement>>> sets;
        join::TJoin<Label, CostModel, APTED> bib_join;
        bib_join.convert_trees_to_sets(trees_collection, sets);
        for (const auto threshold : thresholds)
        {
            std::cout << "--- Threshold " << threshold << " candidates\n";
            bib_join.retrieve_candidates(sets, candidates, threshold);
            for (const auto &candidate : candidates)
            {
                auto t1 = std::get<0>(candidate);
                auto t2 = std::get<1>(candidate);
                std::cout << t1 << " -> " << t2 << "\n";
            }
        }
    }

    return 0;
}