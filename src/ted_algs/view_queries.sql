-- Sentiment ted_apted average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_apted_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_apted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment ted_sed average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_sed_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_sed_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_sed NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment ted_cted average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_cted_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_cted_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_cted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM tedk_labelguided NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_touzetd average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM tedk_touzetd NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_apted average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_apted_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_apted_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_apted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_sed average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_sed_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_sed_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_sed NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP ted_cted average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_cted_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_cted_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM ted_cted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM tedk_labelguided NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_touzetd average runtime per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10;
CREATE MATERIALIZED VIEW dblp_touzetd_x_avg_pair_tree_size_y_avg_runtime_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(runtime) AS avg_runtime
  FROM tedk_touzetd NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_threshold_y_sum_runtime CASCADE;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_threshold_y_sum_runtime AS
  SELECT ted_threshold AS ted_threshold, sum(runtime) AS sum_runtime
  FROM tedk_labelguided NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2) <= ted_threshold
  GROUP BY ted_threshold
  ORDER BY ted_threshold
WITH DATA;

-- Sentiment tedk_touzetd runtime sum per threshold.
DROP MATERIALIZED VIEW IF EXISTS sentiment_touzetd_x_threshold_y_sum_runtime CASCADE;
CREATE MATERIALIZED VIEW sentiment_touzetd_x_threshold_y_sum_runtime AS
  SELECT ted_threshold as ted_threshold, sum(runtime) as sum_runtime
  FROM tedk_touzetd NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2) <= ted_threshold
  GROUP BY ted_threshold
  ORDER BY ted_threshold
WITH DATA;

-- Sentiment runtime factor per threshold tedk_labelguided vs. tedk_touzetd.
DROP MATERIALIZED VIEW IF EXISTS sentiment_x_threshold_y_runtime_factor;
CREATE MATERIALIZED VIEW sentiment_x_threshold_y_runtime_factor AS
  SELECT l.ted_threshold, t.sum_runtime/l.sum_runtime AS runtime_factor
  FROM sentiment_labelguided_x_threshold_y_sum_runtime AS l
  JOIN sentiment_touzetd_x_threshold_y_sum_runtime AS t
  ON l.ted_threshold=t.ted_threshold
WITH DATA;

-- Sentiment ted_apted average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_apted_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW sentiment_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM ted_apted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment tedk_labelguided average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM tedk_labelguided NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment Error: (ub-ted)/ted
DROP MATERIALIZED VIEW IF EXISTS sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW sentiment_labelguided_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM sentiment_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 AS l
  JOIN sentiment_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS a
  ON l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;

-- Sentiment ted_cted average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS sentiment_cted_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW sentiment_cted_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM ted_cted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'sentiment_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- Sentiment Error: (ub-ted)/ted
DROP MATERIALIZED VIEW IF EXISTS sentiment_cted_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW sentiment_cted_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM sentiment_cted_x_avg_pair_tree_size_y_avg_ted_k10 AS l
  JOIN sentiment_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS a
  ON l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;

-- DBLP ted_apted average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_apted_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW dblp_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM ted_apted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP tedk_labelguided average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM tedk_labelguided NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  AND ted_threshold = 10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP Error: (ub-ted)/ted
DROP MATERIALIZED VIEW IF EXISTS dblp_labelguided_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW dblp_labelguided_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM dblp_labelguided_x_avg_pair_tree_size_y_avg_ted_k10 AS l
  JOIN dblp_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS a
  ON l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;

-- DBLP ted_cted average ted value per average size in tree pair k10.
DROP MATERIALIZED VIEW IF EXISTS dblp_cted_x_avg_pair_tree_size_y_avg_ted_k10 CASCADE;
CREATE MATERIALIZED VIEW dblp_cted_x_avg_pair_tree_size_y_avg_ted_k10 AS
  SELECT (tree_size_1+tree_size_2)/2.0 AS avg_pair_tree_size, avg(ted_value) AS avg_ted_value
  FROM ted_cted NATURAL JOIN ted_experiment_params
  WHERE dataset_filename = 'dblp_no_www_sorted.bracket'
  AND abs(tree_size_1-tree_size_2)<=10
  GROUP BY avg_pair_tree_size
  ORDER BY avg_pair_tree_size
WITH DATA;

-- DBLP Error: (ub-ted)/ted
DROP MATERIALIZED VIEW IF EXISTS dblp_cted_x_avg_pair_tree_size_y_ted_error_k10;
CREATE MATERIALIZED VIEW dblp_cted_x_avg_pair_tree_size_y_ted_error_k10 AS
  SELECT l.avg_pair_tree_size, a.avg_ted_value/a.avg_pair_tree_size AS dis, (l.avg_ted_value-a.avg_ted_value)/(a.avg_ted_value) AS error
  FROM dblp_cted_x_avg_pair_tree_size_y_avg_ted_k10 AS l
  JOIN dblp_apted_x_avg_pair_tree_size_y_avg_ted_k10 AS a
  ON l.avg_pair_tree_size=a.avg_pair_tree_size
WITH DATA;