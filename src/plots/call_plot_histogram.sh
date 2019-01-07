### FPR
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_bolzano.json --storeplot "./plots/fpr/bolzano_fpr.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_dblp_10000.json --storeplot "./plots/fpr/dblp_fpr_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_python_10000.json --storeplot "./plots/fpr/python_fpr_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_sentiment.json --storeplot "./plots/fpr/sentiment_fpr.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/fpr/fpr_swissprot_10000.json --storeplot "./plots/fpr/swissprot_fpr_10000.pdf"

### Join Times
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_bolzano.json --storeplot "./plots/times/bolzano_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_dblp_10000.json --storeplot "./plots/times/dblp_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_python_10000.json --storeplot "./plots/times/python_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_sentiment.json --storeplot "./plots/times/sentiment_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/times/times_swissprot_10000.json --storeplot "./plots/times/swissprot_times_10000.pdf"

### Candidate Times
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times/cand_times_bolzano.json --storeplot "./plots/cand_times/bolzano_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times/cand_times_dblp_10000.json --storeplot "./plots/cand_times/dblp_cand_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times/cand_times_python_10000.json --storeplot "./plots/cand_times/python_cand_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times/cand_times_sentiment.json --storeplot "./plots/cand_times/sentiment_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times/cand_times_swissprot_10000.json --storeplot "./plots/cand_times/swissprot_cand_times_10000.pdf"

### Candidate Times
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times_conv/cand_times_conv_bolzano.json --storeplot "./plots/cand_times_conv/bolzano_cand_conv_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times_conv/cand_times_conv_dblp_10000.json --storeplot "./plots/cand_times_conv/dblp_cand_conv_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times_conv/cand_times_conv_python_10000.json --storeplot "./plots/cand_times_conv/python_cand_conv_times_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times_conv/cand_times_conv_sentiment.json --storeplot "./plots/cand_times_conv/sentiment_cand_conv_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/cand_times_conv/cand_times_conv_swissprot_10000.json --storeplot "./plots/cand_times_conv/swissprot_cand_conv_times_10000.pdf"

### Candidates
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidates/candidates_bolzano.json --storeplot "./plots/candidates/bolzano_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidates/candidates_dblp_10000.json --storeplot "./plots/candidates/dblp_candidates_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidates/candidates_python_10000.json --storeplot "./plots/candidates/python_candidates_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidates/candidates_sentiment.json --storeplot "./plots/candidates/sentiment_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidates/candidates_swissprot_10000.json --storeplot "./plots/candidates/swissprot_candidates_10000.pdf"

### Candidates LabelHisto, TJoin, TJoin + UB, Result
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidatesub/candidatesub_bolzano.json --storeplot "./plots/candidatesub/bolzano_candidatesub.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidatesub/candidatesub_dblp_10000.json --storeplot "./plots/candidatesub/dblp_candidatesub_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidatesub/candidatesub_python_10000.json --storeplot "./plots/candidatesub/python_candidatesub_10000.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidatesub/candidatesub_sentiment.json --storeplot "./plots/candidatesub/sentiment_candidatesub.pdf"
python3 plot_experiments.py --service ted-exp --config configs_histograms/candidatesub/candidatesub_swissprot_10000.json --storeplot "./plots/candidatesub/swissprot_candidatesub_10000.pdf"