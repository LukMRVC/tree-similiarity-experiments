# Tree Edit Distance similarity join - experiments

## Building

First clone the external libraries. Execute the following from the project's root directory.
```bash
mkdir external
cd external
# Timing library.
git clone git@frosch.cosy.sbg.ac.at:wmann/common-code.git
# Tree Similarity library.
git clone -b develop https://github.com/DatabaseGroup/tree-similarity.git
```

Then execute the following from the project's root directory.
```bash
mkdir build
cd build
cmake ..
make
```

## PostgreSQL

Create a database using the SQL file ``./db/create_db.sql``.

Further, create ``~/.pg_service.conf`` on the machine where you execute the experiments. 
Insert a service for the database used to store experimental results.

## Executing

Use Python3 to run the experiments script by executing the following command from the root directory of the repository. 
```bash
python3 src/join_algs/join_algs_experiments.py --config configs/icde2019/bolzano.json --dataset_path /path_to/ted-datasets/ --service service
```
