### Upperbound
python3 plot_experiments.py --config configs/upperbound/dblp_error.json --storeplot "./plots/dblp_error.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_runtime_k.json --storeplot "./plots/dblp_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_runtime.json --storeplot "./plots/dblp_runtime_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_ted.json --storeplot "./plots/dblp_ted.pdf"

python3 plot_experiments.py --config configs/upperbound/python_runtime_k.json --storeplot "./plots/python_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/python_runtime.json --storeplot "./plots/python_runtime_9305acf1.pdf"

python3 plot_experiments.py --config configs/upperbound/sentiment_error.json --storeplot "./plots/sentiment_error.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_runtime_k.json --storeplot "./plots/sentiment_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_runtime.json --storeplot "./plots/sentiment_runtime_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_ted.json --storeplot "./plots/sentiment_ted.pdf"

python3 plot_experiments.py --config configs/upperbound/swissprot_runtime_k.json --storeplot "./plots/swissprot_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/swissprot_runtime.json --storeplot "./plots/swissprot_runtime_9305acf1.pdf"

### FPR
python3 plot_experiments.py --config configs/fpr/fpr_bolzano.json --storeplot "./plots/bolzano_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_dblp.json --storeplot "./plots/dblp_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_python.json --storeplot "./plots/python_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_sentiment.json --storeplot "./plots/sentiment_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_swissprot.json --storeplot "./plots/swissprot_fpr.pdf"

### Pairs
python3 plot_experiments.py --config configs/pairs/tree_pairs_bolzano.json --storeplot "./plots/bolzano_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_dblp.json --storeplot "./plots/dblp_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_python.json --storeplot "./plots/python_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_sentiment.json --storeplot "./plots/sentiment_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_swissprot.json --storeplot "./plots/swissprot_pairs.pdf"

### Times
python3 plot_experiments.py --config configs/times/times_bolzano.json --storeplot "./plots/bolzano_times.pdf"
python3 plot_experiments.py --config configs/times/times_dblp.json --storeplot "./plots/dblp_times.pdf"
python3 plot_experiments.py --config configs/times/times_python.json --storeplot "./plots/python_times.pdf"
python3 plot_experiments.py --config configs/times/times_sentiment.json --storeplot "./plots/sentiment_times.pdf"
python3 plot_experiments.py --config configs/times/times_swissprot.json --storeplot "./plots/swissprot_times.pdf"
