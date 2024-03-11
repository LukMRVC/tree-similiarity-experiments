//
// Created by lukas on 2/19/24.
//

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <numeric>
#include "sed_tree_index.h"
#include "bracket_notation_parser.h"
#include "node.h"
#include "string_label.h"
#include "unit_cost_model.h"
#include "histogram_converter.h"
#include "label_intersection.h"

using Label = label::StringLabel;
using CostModelLD = cost_model::UnitCostModelLD<Label>;
using LabelDictionary = label::LabelDictionary<Label>;

using TreeCollection = std::vector<node::Node<Label>>;

using Histogram = std::vector<std::pair<int, std::unordered_map<int, int>>>;

int main()
{
    TreeCollection trees_collection;
    parser::BracketNotationParser<Label> bnp;

    bnp.parse_collection(trees_collection, "./simple_trees.txt");
    LabelDictionary ld;
    CostModelLD ucm(ld);

    auto t1 = trees_collection[0];
    auto t2 = trees_collection[1];
    node::TreeIndexLI t1idx;
    node::TreeIndexLI t2idx;
    node::TreeIndexSED sed_index;
    node::index_tree(t1idx, t1, ld, ucm);
    node::index_tree(t2idx, t2, ld, ucm);
    node::index_tree(sed_index, t1, ld, ucm);

    auto li_lb = ted_lb::LabelIntersection<CostModelLD, node::TreeIndexLI>(ucm);
    auto lb = li_lb.ted(t1idx, t2idx);

    std::cout << "LBLINT LB: " << lb << std::endl;

    std::cout << "Preorder traversal: " << std::endl;
    for (const auto &p : sed_index.prel_to_label_id_)
    {
        std::cout << p << " , ";
    }
    std::cout << std::endl;

    std::cout << "Postorder traversal: " << std::endl;
    for (const auto &p : sed_index.postl_to_label_id_)
    {
        std::cout << p << " , ";
    }
    std::cout << std::endl;

    histogram_converter::Converter<Label> hc;
    Histogram label;
    Histogram degree;
    Histogram leaf;
    hc.create_histogram(trees_collection, label, degree, leaf);

    return 0;
}