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
    AND abs(a.tree_size_1-a.tree_size_2)<=10
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
    AND abs(a.tree_size_1-a.tree_size_2)<=10
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
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'sentiment_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- Sentiment tedk_touzetd runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_touzetd AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'sentiment_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- DBLP ted_apted.
DROP MATERIALIZED VIEW IF EXISTS dblp_apted_x_avg_pair_tree_size_y_avg_runtime;
CREATE MATERIALIZED VIEW dblp_apted_x_avg_pair_tree_size_y_avg_runtime AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND abs(a.tree_size_1-a.tree_size_2)<=10
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
    AND abs(a.tree_size_1-a.tree_size_2)<=10
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
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW dblp_labelguided_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- DBLP tedk_touzetd runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS dblp_touzetd_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW dblp_touzetd_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_touzetd AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- Python tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS python_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW python_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'python_ast_sorted.bracket'
    AND a.ted_threshold = 10
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Python tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS python_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW python_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'python_ast_sorted.bracket'
    AND a.ted_threshold = 10
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Python tedk_labelguided k2.
DROP MATERIALIZED VIEW IF EXISTS python_labelguided_x_avg_pair_tree_size_y_avg_runtime_k2;
CREATE MATERIALIZED VIEW python_labelguided_x_avg_pair_tree_size_y_avg_runtime_k2 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'python_ast_sorted.bracket'
    AND a.ted_threshold = 2
    AND abs(a.tree_size_1-a.tree_size_2)<=2
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Python tedk_touzetd k2.
DROP MATERIALIZED VIEW IF EXISTS python_touzetd_x_avg_pair_tree_size_y_avg_runtime_k2;
CREATE MATERIALIZED VIEW python_touzetd_x_avg_pair_tree_size_y_avg_runtime_k2 AS
  SELECT avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM (
    SELECT a.runtime, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'python_ast_sorted.bracket'
    AND a.ted_threshold = 2
    AND abs(a.tree_size_1-a.tree_size_2)<=2
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Python tedk_labelguided runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS python_labelguided_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW python_labelguided_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'python_ast_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- Python tedk_touzetd runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS python_touzetd_x_threshold_y_runtime_sum;
CREATE MATERIALIZED VIEW python_touzetd_x_threshold_y_runtime_sum AS
  SELECT a.ted_threshold as ted_threshold, sum(a.runtime) as sum_runtime, count(a.runtime)
  FROM tedk_touzetd AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'python_ast_sorted.bracket'
  AND abs(a.tree_size_1-a.tree_size_2) <= a.ted_threshold
  GROUP BY a.ted_threshold
  ORDER BY a.ted_threshold
WITH DATA;

-- TED value per pair in collection.

-- Sentiment ted_apted.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_avg_pair_tree_size_y_ted;
CREATE MATERIALIZED VIEW sentiment_apted_x_avg_pair_tree_size_y_ted AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_ted_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_ted_k10 AS
SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
FROM (
  SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'sentiment_sorted.bracket'
  AND a.ted_threshold = 10
  AND abs(a.tree_size_1-a.tree_size_2)<=10
) AS input
GROUP BY avg_pair_tree_size
ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided k15.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_ted_k15;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_ted_k15 AS
SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
FROM (
  SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
  FROM tedk_labelguided AS a, ted_experiment_params AS p
  WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
  AND p.dataset_filename = 'sentiment_sorted.bracket'
  AND a.ted_threshold = 15
  AND abs(a.tree_size_1-a.tree_size_2)<=15
) AS input
GROUP BY avg_pair_tree_size
ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_avg_pair_tree_size_y_ted_k10;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_avg_pair_tree_size_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'sentiment_sorted.bracket'
    AND a.ted_threshold = 10
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_apted.
DROP MATERIALIZED VIEW IF EXISTS dblp_apted_x_avg_pair_tree_size_y_ted;
CREATE MATERIALIZED VIEW dblp_apted_x_avg_pair_tree_size_y_ted AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM ted_apted AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_ted_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_labelguided AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_touzetd k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_touzetd_x_avg_pair_tree_size_y_ted_k10;
CREATE MATERIALIZED VIEW dblp_touzetd_x_avg_pair_tree_size_y_ted_k10 AS
  SELECT avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM (
    SELECT a.ted_value, a.tree_size_1, a.tree_size_2, (a.tree_size_1+a.tree_size_2)/2.0 as avg_pair_tree_size
    FROM tedk_touzetd AS a, ted_experiment_params AS p
    WHERE a.ted_experiment_params_id = p.ted_experiment_params_id
    AND p.dataset_filename = 'dblp_no_www_db8be93_sorted.bracket'
    AND a.ted_threshold = 10
    AND abs(a.tree_size_1-a.tree_size_2)<=10
  ) AS input
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Error: (ub-ted)/ted
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM sentiment_labelguided_x_avg_pair_tree_size_y_ted_k10 AS l, sentiment_apted_x_avg_pair_tree_size_y_ted AS a
  WHERE l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;

DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k15;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k15 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM sentiment_labelguided_x_avg_pair_tree_size_y_ted_k15 AS l, sentiment_apted_x_avg_pair_tree_size_y_ted AS a
  WHERE l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;

DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM dblp_labelguided_x_avg_pair_tree_size_y_ted_k10 AS l, dblp_apted_x_avg_pair_tree_size_y_ted AS a
  WHERE l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;
