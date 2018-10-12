### Upperbound
python3 plot_experiments.py --config configs/upperbound/dblp_error.json --storeplot "./dblp_error.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_runtime_k.json --storeplot "./dblp_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_runtime.json --storeplot "./dblp_runtime_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/dblp_ted.json --storeplot "./dblp_ted.pdf"

python3 plot_experiments.py --config configs/upperbound/python_runtime_k.json --storeplot "./python_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/python_runtime.json --storeplot "./python_runtime.pdf"

python3 plot_experiments.py --config configs/upperbound/sentiment_error.json --storeplot "./sentiment_error.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_runtime_k.json --storeplot "./sentiment_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_runtime.json --storeplot "./sentiment_runtime_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/sentiment_ted.json --storeplot "./sentiment_ted.pdf"

python3 plot_experiments.py --config configs/upperbound/swissprot_runtime_k.json --storeplot "./swissprot_runtime_k_9305acf1.pdf"
python3 plot_experiments.py --config configs/upperbound/swissprot_runtime.json --storeplot "./swissprot_runtime.pdf"

### FPR
python3 plot_experiments.py --config configs/fpr/fpr_bolzano.json --storeplot "./bolzano_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_dblp.json --storeplot "./dblp_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_python.json --storeplot "./python_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_sentiment.json --storeplot "./sentiment_fpr.pdf"
python3 plot_experiments.py --config configs/fpr/fpr_swissprot.json --storeplot "./swissprot_fpr.pdf"

### Pairs
python3 plot_experiments.py --config configs/pairs/tree_pairs_bolzano.json --storeplot "./bolzano_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_dblp.json --storeplot "./dblp_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_python.json --storeplot "./python_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_sentiment.json --storeplot "./sentiment_pairs.pdf"
python3 plot_experiments.py --config configs/pairs/tree_pairs_swissprot.json --storeplot "./swissprot_pairs.pdf"

### Times
python3 plot_experiments.py --config configs/times/times_bolzano.json --storeplot "./bolzano_times.pdf"
python3 plot_experiments.py --config configs/times/times_dblp.json --storeplot "./dblp_times.pdf"
python3 plot_experiments.py --config configs/times/times_python.json --storeplot "./python_times.pdf"
python3 plot_experiments.py --config configs/times/times_sentiment.json --storeplot "./sentiment_times.pdf"
python3 plot_experiments.py --config configs/times/times_swissprot.json --storeplot "./swissprot_times.pdf"
