DROP TABLE IF EXISTS dataset CASCADE;
CREATE TABLE dataset (
  short_name varchar(31) PRIMARY KEY,
  filename varchar(127) NOT NULL,
  short_description text,
  number_trees integer NOT NULL,
  avg_tree_size integer,
  min_tree_size integer,
  max_tree_size integer
);

DROP TABLE IF EXISTS algorithm CASCADE;
CREATE TABLE algorithm (
  short_name varchar(31) PRIMARY KEY,
  filename varchar(127) NOT NULL,
  short_description text
);

-- Common algorithm attributes.
--
-- IMPORTANT: Apply to all if modified.
--
-- execution_id serial PRIMARY KEY,
-- experiments_version varchar(127),
-- experiments_timestamp timestamp,
-- dataset_short_name varchar(127) REFERENCES dataset(short_name),
-- dataset_parsing_time bigint,
-- algorithm_short_name varchar(31) REFERENCES algorithm(short_name),
-- algorithm_version varchar(127),
-- threshold decimal,
-- result_set_size integer,

DROP TABLE IF EXISTS naive_self_join;
CREATE TABLE naive_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  dataset_short_name varchar(127) REFERENCES dataset(short_name),
  dataset_parsing_time bigint,
  algorithm_short_name varchar(31) REFERENCES algorithm(short_name),
  algorithm_version varchar(127),
  threshold decimal,
  result_set_size integer,
  -- Algorithm-specific attributes.
  verification_candidates integer, -- All pairs of trees that the join looks at and verifies.
  verification_time bigint -- Total time of the join.
);

DROP TABLE IF EXISTS allpairs_baseline_self_join;
CREATE TABLE allpairs_baseline_self_join (
  -- Common attributes.
  execution_id serial PRIMARY KEY,
  experiments_version varchar(127),
  experiments_timestamp timestamp,
  dataset_short_name varchar(127) REFERENCES dataset(short_name),
  dataset_parsing_time bigint,
  algorithm_short_name varchar(31) REFERENCES algorithm(short_name),
  algorithm_version varchar(127),
  threshold decimal,
  result_set_size integer,
  -- Algorithm-specific attributes.
  tree_to_set_time bigint,
  filter_touched_pairs integer, -- Pairs of trees that the filter looks at.
  filter_verification_candidates integer, -- Pairs of trees resulting from filter only.
  filter_time bigint, -- Total time of the filter and its verification step.
  verification_candidates integer, -- Pairs of trees resulting after verification step of the filter.
  verification_time bigint -- TED verification time.
);
