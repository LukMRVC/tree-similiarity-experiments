DROP TABLE IF EXISTS dataset CASCADE;
CREATE TABLE dataset (
  filename varchar(127) PRIMARY KEY,
  number_trees integer,
  avg_tree_size integer,
  min_tree_size integer,
  max_tree_size integer
);

-- Common algorithm attributes.
--
-- IMPORTANT: Apply to all if modified.
--
-- execution_id serial PRIMARY KEY,
-- experiments_version varchar(127),
-- experiments_timestamp timestamp,
-- dataset_filename varchar(127) REFERENCES dataset(filename),
-- dataset_parsing_time bigint,
-- algorithm_name varchar(127),
-- algorithm_version varchar(127),
-- threshold decimal,
-- result_set_size integer,

DROP TABLE IF EXISTS naive_self_join;
CREATE TABLE naive_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  result_set_size integer,
  -- Algorithm-specific attributes.
  verification_candidates integer,
  verification_time bigint
);

DROP TABLE IF EXISTS allpairs_baseline_self_join;
CREATE TABLE allpairs_baseline_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  dataset_filename varchar(127) REFERENCES dataset(filename),
  dataset_parsing_time bigint,
  algorithm_version varchar(127),
  threshold decimal,
  result_set_size integer,
  -- Algorithm-specific attributes.
  tree_to_set_time bigint,
  length_filter_time bigint,
  length_filter_candidates integer,
  length_filter_verification_time bigint,
  verification_candidates integer,
  verification_time bigint
);
