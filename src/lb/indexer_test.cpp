//
// Created by lukas on 2/19/24.
//

#include "bracket_notation_parser.h"
#include "candidate_index.h"
#include "node.h"
#include "string_label.h"
#include "touzet_kr_set_tree_index.h"
#include "tree_indexer.h"
#include "unit_cost_model.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using Label = label::StringLabel;
using CostModelLD = cost_model::UnitCostModelLD<Label>;
using LabelDictionary = label::LabelDictionary<Label>;
using TedTouzet =
    ted::TouzetKRSetTreeIndex<CostModelLD, node::TreeIndexTouzetKRSet>;

using TreeCollection = std::vector<node::Node<Label>>;

using Histogram = std::vector<std::pair<int, std::unordered_map<int, int>>>;

double
struct_ted(std::pair<int, std::vector<label_set_converter::LabelSetElement>> t1,
           std::pair<int, std::vector<label_set_converter::LabelSetElement>> t2,
           int k) {
  auto bigger = std::max(t1.first, t2.first);
  auto overlap = 0;
  candidate_index::CandidateIndex ci;

  for (auto &t1label : t1.second) {
    for (auto &t2label : t2.second) {
      if (t1label.id == t2label.id) {
        overlap += ci.structural_mapping(t1label, t2label, k);
        /*
        std::reference_wrapper<label_set_converter::LabelSetElement> lset =
        std::ref(t1label);
        std::reference_wrapper<label_set_converter::LabelSetElement> rset =
        std::ref(t2label);

        if (t2label.weight < t1label.weight) {
            lset = std::ref(t2label);
            rset = std::ref(t1label);
        }

        auto rset_start = 0;
        for (int i = 0; i < lset.get().weight; ++i) {
            auto &lnode = lset.get().struct_vect[i];
            int left_side_k_window = std::max(0, lnode.postorder_id - k);

            while (rset_start < rset.get().struct_vect.size() &&
                   rset.get().struct_vect[rset_start].postorder_id <
        left_side_k_window) {
                ++rset_start;
            }

            if (rset_start == rset.get().struct_vect.size()) {
                break;
            }

            for (int j = rset_start; j < rset.get().weight; ++j) {
                auto &rnode = rset.get().struct_vect[j];
                // postorder lowerbound
                if (rnode.postorder_id > k + lnode.postorder_id) {
                    break;
                }

                if (abs(rnode.number_nodes_left - lnode.number_nodes_left) +
                    abs(rnode.number_nodes_right - lnode.number_nodes_right) +
                    abs(rnode.number_nodes_ancestor -
                        lnode.number_nodes_ancestor) +
                    abs(rnode.number_nodes_descendant -
                        lnode.number_nodes_descendant) <= k) {
                    ++overlap;
                    break;
                }
            }
        }
        */

        //                for (const auto &n1: t1label.struct_vect) {
        //                    for (const auto &n2: t2label.struct_vect) {
        //                        auto tau_valid = (abs(n1.number_nodes_ancestor
        //                        - n2.number_nodes_ancestor)
        //                                          +
        //                                          abs(n1.number_nodes_descendant
        //                                          -
        //                                          n2.number_nodes_descendant)
        //                                          + abs(n1.number_nodes_left -
        //                                          n2.number_nodes_left)
        //                                          + abs(n1.number_nodes_right
        //                                          - n2.number_nodes_right)) <=
        //                                          k;
        //
        //                        overlap += tau_valid ? 1 : 0;
        //                    }
        //                }
        break;
      }
    }
  }

  return bigger - overlap;
}

int main() {
  TreeCollection trees_collection;
  parser::BracketNotationParser<Label> bnp;
  using TedTouzet =
      ted::TouzetKRSetTreeIndex<CostModelLD, node::TreeIndexTouzetKRSet>;

  auto tree = bnp.parse_single(
      R"#({title{Design of Reverse Converters for a New Flexible RNS Five-Moduli Set \\(\\\{ 2^k, 2^n-1, 2^n+1, 2^\{n+1\}-1, 2^\{n-1\}-1 \\\}\\) (}{i{n}}{ Even).}})#");

  bnp.parse_collection(trees_collection,
                       "/home/lukas/school/tree-statistics/resources/workloads/"
                       "traditional/sentiment_sorted.bracket");
  //    bnp.parse_collection(trees_collection, "./simple_trees.txt");
  LabelDictionary ld;
  CostModelLD ucm(ld);

  node::TreeIndexTouzetKRSet t1idx;
  node::TreeIndexTouzetKRSet t2idx;

  std::ifstream tfs("/home/lukas/school/tree-statistics/resources/workloads/"
                    "traditional/sentiment_sorted.bracket");

  auto lineidx = 0;
  std::string line;
  while (std::getline(tfs, line)) {
    if (!bnp.validate_input(line)) {
      std::cerr << "Invalid input on line " << lineidx << std::endl;
    }

    if (lineidx == 1099) {
      std::cout << line << std::endl;
    }
    lineidx++;
  }

  //   if (!bnp.validate_input("{a}}")) {
  //     std::cerr << "Invalid input" << std::endl;
  //     return 1;
  //   }

  auto n1 = bnp.parse_single("{2{2 Who}{2{2{2 is}{2{2{2 the}{2 audience}}{2{2 "
                             "for}{2{2 Cletis}{2 Tout}}}}}}}");
  auto n2 = bnp.parse_single(
      "{4{3{2 A}{3{2{3 rare}{2 and}}{3{2 lightly}{4 entertaining}}}}{3{2{2 "
      "look}{2{2 behind}{2{2{2 the}{2 curtain}}{2{2 that}{3{2{2 separates}{2 "
      "comics}}{3{2 from}{3{2{2 the}{2 people}}{3{4 laughing}{2{2 in}{2{2 "
      "the}{2 crowd}}}}}}}}}}}}}");

  node::index_tree(t1idx, n1, ld, ucm);
  node::index_tree(t2idx, n2, ld, ucm);

  TedTouzet top_diff(ucm);

  auto final_ted = top_diff.ted(t1idx, t2idx);
  std::cout << "Final TED: " << final_ted << std::endl;

  /*
  auto t1 = trees_collection[0];
  auto t2 = trees_collection[1];
  node::TreeIndexLI t1idx;
  node::TreeIndexLI t2idx;
  node::TreeIndexSED sed_index;
  node::index_tree(t1idx, t1, ld, ucm);
  node::index_tree(t2idx, t2, ld, ucm);
  node::index_tree(sed_index, t1, ld, ucm);
  std::vector<std::pair<int, int>> candidates;

  node::TreeIndexTouzetKRSet t1setidx;
  node::index_tree(t1setidx, t1, ld, ucm);
  node::TreeIndexTouzetKRSet t2setidx;
  node::index_tree(t2setidx, t2, ld, ucm);
  TedTouzet top_diff(ucm);
  join::TJoinTI<Label, TedTouzet> Tjoin;
  std::vector<join::JoinResultElement> join_results;

  std::vector<std::pair<int, std::vector<label_set_converter::LabelSetElement>>>
  sets_collection;
*/
  //    Tjoin.execute_join(
  //            trees_collection,
  //            sets_collection,
  //            candidates,
  //            join_results,
  //            10
  //    );

  //   label_set_converter::Converter<Label> lsc;
  //   lsc.assignFrequencyIdentifiers(trees_collection, sets_collection);

  //   const auto k = 10;
  //   //    Tjoin.convert_trees_to_sets(trees_collection, sets_collection);
  //   //    Tjoin.retrieve_candidates(sets_collection, candidates, k);
  //   //    for (const auto & pair: candidates) {
  //   //        std::cout << pair.first << "," << pair.second << "\n";
  //   //    }

  //   // TODO: Uncomment
  //   for (auto i = 0; i < sets_collection.size(); ++i) {
  //     for (auto j = i + 1; j < sets_collection.size(); ++j) {
  //       auto lb = struct_ted(sets_collection[i], sets_collection[j], k);
  //       if (lb <= k) {
  //         std::cout << i << "," << j << "\n";
  //       }
  //     }
  //   }

  /*sets_collection.clear();
  auto li_lb = ted_lb::LabelIntersection<CostModelLD, node::TreeIndexLI>(ucm);
  auto lb = li_lb.ted(t1idx, t2idx);

  std::cout << "LBLINT LB: " << lb << std::endl;

  std::cout << "Preorder traversal: " << std::endl;
  for (const auto &p: sed_index.prel_to_label_id_) {
      std::cout << p << " , ";
  }
  std::cout << std::endl;

  std::cout << "Postorder traversal: " << std::endl;
  for (const auto &p: sed_index.postl_to_label_id_) {
      std::cout << p << " , ";
  }
  std::cout << std::endl;
  std::vector<std::pair<int, int>> candidates;
  std::vector<std::chrono::microseconds> times;

  TedTouzet top_diff(ucm);
  join::TJoinTI<Label, TedTouzet> tjoin;
  std::vector<join::JoinResultElement> join_results;

  tjoin.execute_join(trees_collection, sets_collection, candidates,
  join_results, 30);

  histogram_converter::Converter<Label> hc;
  Histogram label;
  Histogram degree;
  Histogram leaf;
  hc.create_histogram(trees_collection, label, degree, leaf);
  histo_candidate_index::CandidateIndex ci;
  ci.lookup(label, degree, leaf, candidates, hc.get_number_of_labels(), 6,
  times);*/

  return 0;
}