DROP TABLE IF EXISTS dataset CASCADE;
CREATE TABLE dataset (
  filename varchar(127) PRIMARY KEY,
  short_description text,
  number_trees bigint NOT NULL,
  avg_tree_size bigint,
  min_tree_size bigint,
  max_tree_size bigint,
  number_labels bigint
);
--
-- DROP TABLE IF EXISTS algorithm CASCADE;
-- CREATE TABLE algorithm (
--   short_name varchar(31) PRIMARY KEY,
--   filename varchar(127) NOT NULL,
--   short_description text
-- );

-- Common algorithm attributes.
--
-- IMPORTANT: Apply to all if modified.
--
-- execution_id serial PRIMARY KEY,
-- experiments_version varchar(127),
-- experiments_timestamp timestamp,
-- hostname varchar(127),
-- dataset_filename varchar(127) REFERENCES dataset(filename),
-- dataset_parsing_time bigint,
-- algorithm_version varchar(127),
-- threshold decimal,
-- sum_subproblems bigint,
-- result_set_size bigint,
-- sum_subproblem_optimum bigint,
-- optimum_time bigint,
-- inv_list_lookups bigint,

DROP TABLE IF EXISTS naive_self_join;
CREATE TABLE naive_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  verification_candidates bigint, -- All pairs of trees that the join looks at and verifies.
  verification_time bigint -- Total time of the join.
);

DROP TABLE IF EXISTS allpairs_self_join;
CREATE TABLE allpairs_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  similarity_function varchar(127),
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  filter_touched_pairs bigint, -- Pairs of trees that the filter looks at.
  filter_verification_candidates bigint, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS allpairs_multiset_baseline_self_join;
CREATE TABLE allpairs_multiset_baseline_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  similarity_function varchar(127),
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  filter_touched_pairs bigint, -- Pairs of trees that the filter looks at.
  filter_verification_candidates bigint, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS allpairs_multiset_dsf_self_join;
CREATE TABLE allpairs_multiset_dsf_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  similarity_function varchar(127),
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  filter_touched_pairs bigint, -- Pairs of trees that the filter looks at.
  filter_verification_candidates bigint, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS allpairs_multiset_two_layer_self_join;
CREATE TABLE allpairs_multiset_two_layer_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  similarity_function varchar(127),
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  filter_touched_pairs bigint, -- Pairs of trees that the filter looks at.
  filter_verification_candidates bigint, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS partition_based_self_join;
CREATE TABLE partition_based_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  sum_subproblems bigint,
  result_set_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  inv_list_lookups bigint,
  -- Algorithm-specific attributes.
  similarity_function varchar(127),
  verification_algorithm varchar(31),
  tree_to_binary_tree_time bigint,
  filter_touched_pairs bigint, -- Pairs of trees that the filter looks at.
  filter_verification_candidates bigint, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);
