-- Average runtime per average size in tree pair.

-- Sentiment ted_apted.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_avg_pair_tree_size_y_avg_runtime;
CREATE MATERIALIZED VIEW sentiment_apted_x_avg_pair_tree_size_y_avg_runtime AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_apted.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_avg_pair_tree_size_y_avg_runtime;
CREATE MATERIALIZED VIEW dblp_apted_x_avg_pair_tree_size_y_avg_runtime AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- TED value per pair in collection.

-- Sentiment ted_apted.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_pair_id_y_ted;
CREATE MATERIALIZED VIEW sentiment_apted_x_pair_id_y_ted AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_pair_id_y_ted_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_pair_id_y_ted_k10 AS
SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
FROM (
  SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'sentiment_sorted.bracket'
  AND a.ted_threshold = 10
) AS input
GROUP BY avg_pair_tree_size
ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_pair_id_y_ted_k10;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_pair_id_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_apted.
DROP MATERIALIZED VIEW IF EXISTS dblp_apted_x_pair_id_y_ted;
CREATE MATERIALIZED VIEW dblp_apted_x_pair_id_y_ted AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_pair_id_y_ted_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_pair_id_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_touzetd_x_pair_id_y_ted_k10;
CREATE MATERIALIZED VIEW dblp_touzetd_x_pair_id_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;
