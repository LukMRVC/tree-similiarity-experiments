//
// Created by lukas on 10/3/23.
//

#include "ted_lb_experiments.h"
#include <thread>
#include <future>
#include <fstream>

rlim_t increase_stack_size(rlim_t &);

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

using result_t = std::vector<std::tuple<size_t, size_t, double>>;

result_t execute_dataset_apted(const std::string & dataset_path) {
    using Label = label::StringLabel;
    using CostModel = cost_model::UnitCostModel<Label>;
    //  Use APTED to compute actual tree distances at least once between all trees <- this is going to take a lot of time
    //  Computing all these distances is necessary, so we can determine the success rate of the filter
    //  The computation is going to take a lot of time, might as well run it in parallel :]
    using APTED = ted::APTED<Label, CostModel>;
    // PARSE INPUT
    // The input is parsed once for the entire experiment.
    std::vector<node::Node<Label>> trees_collection;
    parser::BracketNotationParser bnp;
    std::cout << "Parsing tree collection" << std::endl;
    bnp.parse_collection(trees_collection, dataset_path);
    std::cout << "Parsing done" << std::endl;

    APTED apted_alg;

    auto max_threads = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    std::vector<std::future<result_t>> future_results;
    auto chunk_size = (trees_collection.size() + max_threads - 1) / max_threads;

    for (size_t i = 0; i < max_threads; i++)
    {
        std::promise<result_t> promise;
        future_results.push_back(promise.get_future());
        threads.emplace_back([chunk_size, i, &trees_collection](std::promise<result_t> &&p)
                             {
                                 APTED alg;
                                 result_t results;
                                 for (size_t j = i * chunk_size; j < chunk_size && j < trees_collection.size(); j++)
                                 {
                                     for (size_t k = 0; k < trees_collection.size(); k++)
                                     {
                                         auto ted = alg.apted_ted(
                                                 trees_collection[i],
                                                 trees_collection[j]
                                         );
                                         results.emplace_back(j, k, ted);
                                     }

                                 }
                                 p.set_value(results); },
                             std::move(promise));
        std::cout << "Created thread " << i << std::endl;
    }

    std::vector<std::tuple<size_t, size_t, double>> all_ted_results;
    for (auto &future : future_results)
    {
        auto return_val = future.get();
        all_ted_results.insert(all_ted_results.end(), return_val.begin(), return_val.end());
    }

    std::cout << "Joining " << threads.size() << " running threads" << std::endl;
    for (auto &t : threads)
    {
        t.join();
    }

    return all_ted_results;
}


int main(int argc, char *argv[])
{
    std::vector<std::string> args(argv, argv + argc);
    if (args.size() < 3)
    {
        std::cerr << "Missing arguments for datasets base path and datasets\n";
        exit(1);
    }


    // path to dataset file containing trees in BN (Bracket notation)
    std::string datasets_base_path = args.at(1);
    if  (*datasets_base_path.end() != '/') {
        datasets_base_path.append("/");
    }

    std::string datasets = args.at(2);

    std::vector<std::string> datasets_paths = split(datasets, ',');
    for (auto & dataset : datasets_paths) {
        dataset.insert(0, datasets_base_path);
    }

    // now increase stack size
    rlim_t new_stack_size = 16777216;
    if (increase_stack_size(new_stack_size) != new_stack_size)
    {
        std::cerr << "Unable to increase stack size!" << std::endl;
        exit(1);
    }

    for (const auto & dataset_path: datasets_paths) {
        std::string output_file_path(dataset_path);
        auto pos = output_file_path.rfind('/');
        output_file_path.erase(output_file_path.begin() + pos, output_file_path.end());
        output_file_path.insert(pos, "/distances.csv");

        auto all_ted_results = execute_dataset_apted(dataset_path);
        std::cout << "Totally got " << all_ted_results.size() << " results, writing to file\n";

        std::ofstream output(output_file_path);
        for (auto &result : all_ted_results)
        {
            auto t1 = std::get<0>(result);
            auto t2 = std::get<1>(result);
            auto ted = std::get<2>(result);
            output << "TED between T" << t1 << " and T" << t2 << " is " << ted << "\n";
        }
    }



    return 0;
}

rlim_t increase_stack_size(rlim_t &new_stack_size)
{
    // Define and object of structure rlimit.
    struct rlimit rl;
    // First get the time limit on CPU
    getrlimit(RLIMIT_STACK, &rl);
    // printf("\n Default value is : %lld\n", (long long int)rl.rlim_cur);
    // Change the time limit
    rl.rlim_cur = new_stack_size;
    // Now call setrlimit() to set the
    // changed value.
    setrlimit(RLIMIT_STACK, &rl);
    // Again get the limit and check
    getrlimit(RLIMIT_STACK, &rl);
    // printf("\n Default value now is : %lld\n", (long long int)rl.rlim_cur);
    return rl.rlim_cur;
}
