# Tree Edit Distance Experiments

Currently the experiments framework contains stand-alone tree edit distance
and tree similarity join algorithms.

Follow the instructions below to reproduce the environment and the experiments.

## ICDE 2019 Reproducibility

This repository contains experiments of our ICDE 2019 paper
[Effective Filters and Linear Time Verification for Tree Similarity Joins](http://eplus.uni-salzburg.at/obvusboa/download/pdf/4486886).

To reproduce the experiments of the ICDE 2019 paper, checkout the tag
`icde2019` of this and
[Tree Similarity library](https://github.com/DatabaseGroup/tree-similarity/tree/develop)
repositories.

Obtain datasets from our
[Datasets repository](https://frosch.cosy.sbg.ac.at/mpawlik/ted-datasets).

Execute the experiments with all config files in `configs/icde2019` directory.
See execution details below. You may need to modify `--dataset_path` parameter
value when executing the experiments.

For LGM Upper Bound and BSM verification experiment, certain views must present
in the databse. After executing all experiments, execute `src/ted_algs/view_queries.sql`
on the database holding the experiment results.

Plot the results using `src/plots/create_all_plots.sh` file from `src/plots/`
directory.

## Build the project

After cloning the repository, clone the external libraries to `external`
subdirectory.

```bash
mkdir external
cd external
```
Clone the Timing library for runtime measurements.

```bash
git clone git@frosch.cosy.sbg.ac.at:wmann/common-code.git
```

Clone the Tree Similarity library with the algorithms (the `develop` branch
is currently the most recent).

```bash
git clone --branch develop https://github.com/DatabaseGroup/tree-similarity.git
```

Then execute the following from the project's root directory.
```bash
mkdir build
cd build
cmake ..
make
```

## Prepare a PostgreSQL database for storing the results

Install [PostgreSQL](https://www.postgresql.org/).

Create a database using the SQL file ``db/create_db.sql``.

Create a service file ``~/.pg_service.conf`` on the machine where you execute
the experiments. The service file holds the connection details to the database
where the results will be stored. An example service file looks as follows.

```
[ted-exp]
host=mydb.sbg.ac.at
port=5432
user=ted
password=letmethrough
dbname=ted_experiments
```

Executing experiments requires dataset details to be present in the `dataset`
table. Visit our
[Datasets repository](https://frosch.cosy.sbg.ac.at/mpawlik/ted-datasets)
to learn how we obtain datasets. Use the `--service service` option of the
`statistics/statistics.py` script to register a dataset in the `dataset` table.


## Executing

We use [Python3](https://www.python.org/) to execute the experiments.

### TED Join

The script `src/join_algs/join_algs_experiments.py` executes tree similarity
join experiments.

It uses a config JSON file to specify the experiment parameters. Example config
files can be found in `configs/icde2019` directory.

Example experiment execution can be performed as follows.

```bash
python3 src/join_algs/join_algs_experiments.py --config configs/icde2019/bolzano.json --dataset_path /path_to/ted-datasets/ --service service
```

### TED Algorithms

The script `src/ted_algs/ted_algs_experiments.py` executes tree similarity
join experiments.

It uses a config JSON file to specify the experiment parameters. Example config
files can be found in `configs/icde2019/upperbound` directory.

Example experiment execution can be performed as follows.

```bash
python3 src/join_algs/ted_algs_experiments.py --config configs/icde2019/upperbound/sentiment.json --dataset_path /path_to/ted-datasets/ --service service
```
