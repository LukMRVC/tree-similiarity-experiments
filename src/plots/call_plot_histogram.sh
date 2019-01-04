### FPR
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_bolzano.json --storeplot "./plots/bolzano_fpr.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_dblp_10000.json --storeplot "./plots/dblp_fpr_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_python_10000.json --storeplot "./plots/python_fpr_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_sentiment.json --storeplot "./plots/sentiment_fpr.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_swissprot_10000.json --storeplot "./plots/swissprot_fpr_10000.pdf"

### Times
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_bolzano.json --storeplot "./plots/bolzano_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_dblp_10000.json --storeplot "./plots/dblp_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_python_10000.json --storeplot "./plots/python_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_sentiment.json --storeplot "./plots/sentiment_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_swissprot_10000.json --storeplot "./plots/swissprot_times_10000.pdf"