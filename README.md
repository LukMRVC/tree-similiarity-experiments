# Tree Edit Distance similarity join - experiments

## Building

First clone the external libraries. Execute the following from the project's root directory.
```bash
mkdir external
cd external
# Timing library.
git clone git@frosch.cosy.sbg.ac.at:wmann/common-code.git
# Tree Similarity library.
git clone git@frosch.cosy.sbg.ac.at:thuetter/tree-similarity-private.git
```

Then execute the following from the project's root directory.
```bash
mkdir build
cd build
cmake ..
make
```
## PostgreSQL

Create ``~/.pg_service.conf`` on the machine where you execute the experiments.
