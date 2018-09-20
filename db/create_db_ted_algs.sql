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
  runtime bigint
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
  runtime bigint
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
  runtime bigint,
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
  runtime bigint,
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
  runtime bigint,
  ted_threshold decimal
);