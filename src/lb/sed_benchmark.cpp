#include <iostream>
#include <fstream>
#include <ctime>
#include <sys/resource.h>
#include <string>
#include <vector>
#include <limits>
#include <unordered_set>
#include "timing.h"
#include "node.h"
#include "string_label.h"
#include "unit_cost_model.h"
#include "zhang_shasha_tree_index.h"
#include "sed_tree_index.h"
#include "bracket_notation_parser.h"
#include <chrono>
#include <algorithm>
#include <numeric>

uint32_t sed_library(const std::string &a, const std::string &b) {
    const std::string *s1 = &a;
    const std::string *s2 = &b;

    if (a.size() > b.size()) {
        s1 = &b;
        s2 = &a;
    }

    std::vector<uint32_t> cp(s2->size() + 1);
    std::vector<uint32_t> cc(s2->size() + 1);

//    // Edit distance on postorder traversal.
//    for (int j = 0; j <= kT2Size; ++j) {
//        cp[j] = j;
//    }

    std::iota(cp.begin(), cp.end(), 0);

    for (int i = 1; i <= s1->size(); ++i) {
        cc[0] = i;
        for (int j = 1; j <= s2->size(); ++j) {
            char c1 = (*s1)[i - 1];
            char c2 = (*s2)[j - 1];
            cc[j] = std::min({
                                     cp[j - 1] + (c1 != c2),
                                     cc[j - 1] + 1,
                                     cp[j] + 1
                             });
        }
        std::swap(cp, cc);
    }
    return cp[b.size()];
}

uint32_t sed_improved(const char *a, const char *b, size_t alen, size_t blen) {

    if (alen > blen) {
        std::swap(a, b);
        std::swap(alen, blen);
    }

    std::vector<uint32_t> cache(blen + 1);

//    // Edit distance on postorder traversal.
//    for (int j = 0; j <= kT2Size; ++j) {
//        cp[j] = j;
//    }

    std::iota(cache.begin(), cache.end(), 1);

    uint32_t result = blen;
    for (int i = 0; i < alen; ++i) {
        char c1 = a[i];
        uint32_t dist_b = i;
        for (int j = 0; j < blen; ++j) {
            char c2 = b[j];
            auto dist_a = dist_b + (c1 != c2);
            dist_b = cache[j];
            result = std::min({result + 1, dist_a, dist_b + 1});
            cache[j] = result;
        }
    }

    return result;
}

uint32_t sed_bounded(const char *a, const char *b, int64_t alen, int64_t blen, int64_t k) {
    if (alen > blen) {
        std::swap(a, b);
        std::swap(alen, blen);
    }

    // suffix trimming
    while (alen > 0 && a[alen - 1] == b[blen - 1]) {
        alen--;
        blen--;
    }

    // prefix trimming
    while (alen > 0 && *a == *b) {
        a++;
        b++;
        alen--;
        blen--;
    }
    k = std::min(blen, k);
    // if the shorter string is gone, return b_size or threshold
    if (alen == 0) {
        return k;
    }

    auto size_d = blen - alen;

    if (size_d > k) {
        return k;
    }

    int64_t ZERO_K = std::min(k, alen) / 2 + 2;
    auto array_size = size_d + ZERO_K * 2 + 2;

    std::vector<int64_t> current_row(array_size, -1);
    std::vector<int64_t> next_row(array_size, -1);

    int64_t i = 0;
    int64_t condition_row = size_d + ZERO_K;
    int64_t end_max = condition_row * 2;

    do {
        i++;

        std::swap(current_row, next_row);

        int64_t start;
        int64_t previous_cell;
        int64_t current_cell = -1;
        int64_t next_cell;

        if (i <= ZERO_K) {
            start = -i + 1;
            next_cell = i - 2;
        } else {
            start = i - ZERO_K * 2 + 1;
            next_cell = current_row[ZERO_K + start];
        }

        int64_t end;
        if (i <= condition_row) {
            end = i;
            next_row[ZERO_K + i] = -1;
        } else {
            end = end_max - i;
        }

        for (int64_t q = start, row_index = start + ZERO_K; q < end; q++, row_index++) {
            previous_cell = current_cell;
            current_cell = next_cell;
            next_cell = current_row[row_index + 1];

            int64_t t = std::max({
                                         current_cell + 1,
                                         previous_cell,
                                         next_cell + 1
                                 });

            while (t < alen && t + q < blen && a[t] == b[t + q]) {
                t++;
            }

            next_row[row_index] = t;
        }
    } while (next_row[condition_row] < alen && i <= k);

    return i - 1;
}

int main() {
    using namespace std::chrono;
    std::ifstream input_f("./inputs.txt");
    std::ifstream search_f("./search.txt");
    std::cout << "Reading files\n";

    std::vector<std::string> input;
    std::vector<std::string> search;

    std::string line;
    while (std::getline(input_f, line)) {
        input.emplace_back(line);
    }

    while (std::getline(search_f, line)) {
        search.emplace_back(line);
    }
    std::vector<uint32_t> distances;
    distances.reserve(input.size() * search.size() * 2);

//    auto start = high_resolution_clock::now();
//    for (const auto & s : search) {
//        for (const auto & i : input) {
//            distances.emplace_back(sed_library(s, i));
//        }
//    }
//    auto duration = duration_cast<milliseconds>(high_resolution_clock::now() - start);
//
//    std::cout << "Total: " << distances.size() << " distances computed\n";
//    std::cout << "SED-Lib done in " << duration.count() << "ms\n";
//
//    distances.clear();
//    distances.reserve(input.size() * search.size() * 2);
//    auto t2 = high_resolution_clock::now();
//    for (const auto & s : search) {
//        for (const auto & i : input) {
//            distances.emplace_back(sed_improved(s.c_str(), i.c_str(), s.size(), i.size()));
//        }
//    }
//    auto d2 = duration_cast<milliseconds>(high_resolution_clock::now() - t2);
//
//    std::cout << "Total: " << distances.size() << " distances computed\n";
//    std::cout << "SED-Improved done in " << d2.count() << "ms\n";

    distances.clear();
    distances.reserve(input.size() * search.size() * 2);
    auto t3 = high_resolution_clock::now();
    for (const auto &s: search) {
        for (const auto &i: input) {
            distances.emplace_back(sed_bounded(s.c_str(), i.c_str(), s.size(), i.size(), 30));
        }
    }
    auto d3 = duration_cast<milliseconds>(high_resolution_clock::now() - t3);

    std::cout << "Total: " << distances.size() << " distances computed\n";
    std::cout << "SED-Bounded done in " << d3.count() << "ms\n";

    std::ofstream output("./output.txt");
    for (const auto &d: distances) {
        output << d << "\n";
    }

    return 0;
}