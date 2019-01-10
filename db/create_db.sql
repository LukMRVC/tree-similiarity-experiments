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
-- join_result_size bigint,
-- sum_subproblem_optimum bigint,
-- optimum_time bigint

DROP TABLE IF EXISTS naive_join;
CREATE TABLE naive_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  verification_candidates bigint, -- All pairs of trees that the join looks at and verifies.
  verification_time bigint -- Total time of the join.
);

DROP TABLE IF EXISTS t_join;
CREATE TABLE t_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS tang_join;
CREATE TABLE tang_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_binary_tree_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS binary_branches_join;
CREATE TABLE binary_branches_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS label_histogram_join;
CREATE TABLE label_histogram_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS leaf_distance_histogram_join;
CREATE TABLE leaf_distance_histogram_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS degree_histogram_join;
CREATE TABLE degree_histogram_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS histogram_join;
CREATE TABLE histogram_join (
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
  join_result_size bigint,
  sum_subproblem_optimum bigint,
  optimum_time bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  tree_to_set_time bigint,
  inv_list_lookups bigint, -- Pairs of trees that the index looks at.
  index_verification_candidates bigint, -- Pairs of trees resulting from index only.
  index_time bigint, -- Total time of the index and its verification step.
  verification_candidates bigint, -- Pairs of trees resulting after verification step of the index.
  verification_time bigint, -- TED verification time.
  upperbound varchar(127), -- string that defines which upperbound is used
  upperbound_time bigint, -- Time spent to compute upperbounds for verification candidates.
  upperbound_pruned bigint -- Number of pairs in the result set by upperbound computation.
);

DROP TABLE IF EXISTS guha_join;
CREATE TABLE guha_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  join_result_size bigint,
  -- Algorithm-specific attributes.
  verification_algorithm varchar(31),
  vectors_time bigint,
  candidates_time bigint,
  ted_verification_candidates bigint,
  verification_time bigint, -- TED verification time.
  l_t_candidates bigint,
  sed_candidates bigint,
  u_t_result_pairs bigint,
  cted_result_pairs bigint, 
  reference_set_size int
);

-- Parameters of a ted experiment (for normalization):

-- ted_experiment_timestamp timestamp,
-- experiments_source_commit varchar(127),
-- algorithm_source_commit varchar(127),
-- hostname varchar(127),
-- dataset_filename varchar(127) REFERENCES dataset(filename)

-- Common algorithm attributes:
--
-- IMPORTANT: Apply to all if modified.

-- execution_id bigserial PRIMARY KEY,
-- ted_experiment_params_id integer REFERENCES ted_experiment_params(ted_experiment_params_id),
-- tree_id_1 integer,
-- tree_id_2 integer,
-- tree_size_1 integer,
-- tree_size_2 integer,
-- ted_value decimal,
-- subproblems bigint,
-- runtime bigint

-- Only TEDk algs:

-- has_ted_mapping boolean, -- if TRUE ted_value >=0; if FALSE ted_value = NULL
-- ted_threshold decimal,

-- There is a tuple in this table for each execution of the experiments binary.
DROP TABLE IF EXISTS ted_experiment_params CASCADE;
CREATE TABLE ted_experiment_params (
  ted_experiment_params_id serial PRIMARY KEY,
  ted_experiment_timestamp timestamp,
  experiments_source_commit varchar(127),
  algorithm_source_commit varchar(127),
  hostname varchar(127),
  dataset_filename varchar(127) REFERENCES dataset(filename)
);

DROP TABLE IF EXISTS ted_zhangshasha;
CREATE TABLE ted_zhangshasha (
  execution_id bigserial PRIMARY KEY,
  ted_experiment_params_id integer NOT NULL REFERENCES ted_experiment_params(ted_experiment_params_id),
  tree_id_1 integer,
  tree_id_2 integer,
  tree_size_1 integer,
  tree_size_2 integer,
  ted_value decimal,
  subproblems bigint,
  runtime double precision
);

DROP TABLE IF EXISTS ted_apted;
CREATE TABLE ted_apted (
  execution_id bigserial PRIMARY KEY,
  ted_experiment_params_id integer NOT NULL REFERENCES ted_experiment_params(ted_experiment_params_id),
  tree_id_1 integer,
  tree_id_2 integer,
  tree_size_1 integer,
  tree_size_2 integer,
  ted_value decimal,
  subproblems bigint,
  runtime double precision
);

-- Touzet's algorithm - baseline version without the depth-based pruning
DROP TABLE IF EXISTS tedk_touzet;
CREATE TABLE tedk_touzet (
  execution_id bigserial PRIMARY KEY,
  ted_experiment_params_id integer NOT NULL REFERENCES ted_experiment_params(ted_experiment_params_id),
  tree_id_1 integer,
  tree_id_2 integer,
  tree_size_1 integer,
  tree_size_2 integer,
  ted_value decimal,
  subproblems bigint,
  runtime double precision,
  has_ted_mapping boolean, -- if TRUE ted_value >=0; if FALSE ted_value = NULL
  ted_threshold decimal
);

-- Touzet's algorithm - with depth-based pruning
DROP TABLE IF EXISTS tedk_touzetd;
CREATE TABLE tedk_touzetd (
  execution_id bigserial PRIMARY KEY,
  ted_experiment_params_id integer NOT NULL REFERENCES ted_experiment_params(ted_experiment_params_id),
  tree_id_1 integer,
  tree_id_2 integer,
  tree_size_1 integer,
  tree_size_2 integer,
  ted_value decimal,
  subproblems bigint,
  runtime double precision,
  has_ted_mapping boolean, -- if TRUE ted_value >=0; if FALSE ted_value = NULL
  ted_threshold decimal
);

-- Label-guided upper bound
DROP TABLE IF EXISTS tedk_labelguided;
CREATE TABLE tedk_labelguided (
  execution_id bigserial PRIMARY KEY,
  ted_experiment_params_id integer NOT NULL REFERENCES ted_experiment_params(ted_experiment_params_id),
  tree_id_1 integer,
  tree_id_2 integer,
  tree_size_1 integer,
  tree_size_2 integer,
  ted_value decimal,
  subproblems bigint,
  runtime double precision,
  ted_threshold decimal
);