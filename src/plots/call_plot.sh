### Upperbound
python plot_experiments.py --config configs/upperbound/dblp_runtime.json --storeplot "./dblp_runtime.pdf"
python plot_experiments.py --config configs/upperbound/dblp_ted.json --storeplot "./dblp_ted.pdf"
python plot_experiments.py --config configs/upperbound/sentiment_runtime.json --storeplot "./sentiment_runtime.pdf"
python plot_experiments.py --config configs/upperbound/sentiment_ted.json --storeplot "./sentiment_ted.pdf"

### FPR
python plot_experiments.py --config configs/fpr/fpr_bolzano.json --storeplot "./bolzano_fpr.pdf"
python plot_experiments.py --config configs/fpr/fpr_dblp.json --storeplot "./dblp_fpr.pdf"
python plot_experiments.py --config configs/fpr/fpr_python.json --storeplot "./python_fpr.pdf"
python plot_experiments.py --config configs/fpr/fpr_sentiment.json --storeplot "./sentiment_fpr.pdf"
python plot_experiments.py --config configs/fpr/fpr_swissprot.json --storeplot "./swissprot_fpr.pdf"

### Pairs
python plot_experiments.py --config configs/pairs/tree_pairs_bolzano.json --storeplot "./bolzano_pairs.pdf"
python plot_experiments.py --config configs/pairs/tree_pairs_dblp.json --storeplot "./dblp_pairs.pdf"
python plot_experiments.py --config configs/pairs/tree_pairs_python.json --storeplot "./python_pairs.pdf"
python plot_experiments.py --config configs/pairs/tree_pairs_sentiment.json --storeplot "./sentiment_pairs.pdf"
python plot_experiments.py --config configs/pairs/tree_pairs_swissprot.json --storeplot "./swissprot_pairs.pdf"

### Times
python plot_experiments.py --config configs/times/times_bolzano.json --storeplot "./bolzano_times.pdf"
python plot_experiments.py --config configs/times/times_dblp.json --storeplot "./dblp_times.pdf"
python plot_experiments.py --config configs/times/times_python.json --storeplot "./python_times.pdf"
python plot_experiments.py --config configs/times/times_sentiment.json --storeplot "./sentiment_times.pdf"
python plot_experiments.py --config configs/times/times_swissprot.json --storeplot "./swissprot_times.pdf"
