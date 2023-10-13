//
// Created by lukas on 10/3/23.
//

#include "ted_ds_dist.h"
#include <thread>
#include <fstream>
#include <mutex>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/lzma.hpp>
#include <boost/iostreams/copy.hpp>

rlim_t increase_stack_size(rlim_t &);

std::mutex file_lock;

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

void write_to_output(result_t &results, std::ostream &output, size_t collection_size)
{
    auto current_tree = std::get<0>(results[0]);
    size_t i = 0;
    do {
        output << "T:" << current_tree << "\n";
        auto output_size = (collection_size - current_tree);
        for (int j = 0; j < output_size; ++j) {
            auto result = results[j + i];
            auto ted = std::get<2>(result);
            output << ted << "\n";
        }
        i += output_size;
        current_tree = std::get<0>(results[output_size]);
    } while ((i < results.size()));
}

void execute_dataset_apted(const std::string &dataset_path, const std::string &output_file_path)
{
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
    std::cout << "Parsing tree collection for " << dataset_path << std::endl;
    bnp.parse_collection(trees_collection, dataset_path);
    std::cout << "Parsing done" << std::endl;

    APTED apted_alg;

    std::ofstream output_file(output_file_path);
//    boost::iostreams::filtering_streambuf<boost::iostreams::output> outbuf;
//    outbuf.push(boost::iostreams::lzma_compressor());
//    outbuf.push(output_file);
//    std::ostream out(&outbuf);

    auto max_threads = std::thread::hardware_concurrency();

    std::vector<std::thread> threads;
    auto chunk_size = (trees_collection.size() + max_threads - 1) / max_threads;
    std::cout << trees_collection.size() << std::endl;
    /*    std::ofstream output(output_file_path);
        for (auto &result : all_ted_results)
        {
            auto t1 = std::get<0>(result);
            auto t2 = std::get<1>(result);
            auto ted = std::get<2>(result);
            output << t1 << "," << t2 << "," << ted << "\n";
        }*/

    for (size_t i = 0; i < max_threads; i++)
    {
        threads.emplace_back([chunk_size, i, &trees_collection, &output_file]
                             {
                                 APTED alg;
                                 result_t results;
                                 auto stop = std::min((i + 1) * chunk_size, trees_collection.size());
                                 auto actual_chunk_size = stop - (i * chunk_size);

                                 auto progress_ten = (actual_chunk_size + 10 - 1) / 10;

                                 for (size_t j = i * chunk_size; j < stop; j++)
                                 {

                                     for (size_t k = j + 1; k < trees_collection.size(); k++)
                                     {
                                         auto ted = alg.apted_ted(
                                                 trees_collection[j],
                                                 trees_collection[k]
                                         );
                                         results.emplace_back(j, k, ted);
                                     }

                                     if (j % progress_ten == 0) {
                                         std::cout << "Thread " << i << " done: " << 10 * ((j - i * chunk_size) / progress_ten) << "%" << std::endl;
                                         if (!results.empty()) {
                                             file_lock.lock();
                                             write_to_output(results, output_file, trees_collection.size());
                                             file_lock.unlock();
                                             results.clear();
                                         }
                                     }
                                 }

                                 std::cout << "Thread " << i << " done: 100%" << std::endl;
                                 if (!results.empty()) {
                                     file_lock.lock();
                                     write_to_output(results, output_file, trees_collection.size());
                                     file_lock.unlock();
                                     results.clear();
                                 } });
        std::cout << "Created thread " << i << std::endl;
    }

    std::cout << "Joining " << threads.size() << " running threads" << std::endl;
    for (auto &t : threads)
    {
        t.join();
    }

//    boost::iostreams::close(outbuf);
    output_file.close();
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Missing arguments for datasets base path and datasets" << std::endl;
        exit(1);
    }
    std::vector<std::string> args(argv, argv + argc);

    // path to dataset file containing trees in BN (Bracket notation)
    std::string datasets_base_path = args.at(1);
    if (*datasets_base_path.end() != '/')
    {
        datasets_base_path.append("/");
    }

    std::string datasets = args.at(2);

    std::vector<std::string> datasets_paths = split(datasets, ',');
    for (auto &dataset : datasets_paths)
    {
        dataset.insert(0, datasets_base_path);
    }

    // now increase stack size
    rlim_t new_stack_size = 16777216;
    if (increase_stack_size(new_stack_size) != new_stack_size)
    {
        std::cerr << "Unable to increase stack size!" << std::endl;
        exit(1);
    }

    for (const auto &dataset_path : datasets_paths)
    {
        std::string output_file_path(dataset_path);
        auto pos = output_file_path.rfind('/');
        output_file_path.erase(output_file_path.begin() + pos, output_file_path.end());
        output_file_path.insert(pos, "/distances.csv.lzma");

        execute_dataset_apted(dataset_path, output_file_path);
        std::cout << "All results written into file\n";
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
