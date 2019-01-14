rm -rf plots
mkdir plots
cd plots
mkdir fpr
mkdir times
mkdir cand_times
mkdir cand_times_conv
mkdir candidates
mkdir dataset_size
mkdir rsb_rsc
cd dataset_size
mkdir dblp
mkdir swissprot
cd ..
cd ..

### FPR
echo " --- Start FPR --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_bolzano.json --storeplot "./plots/fpr/bolzano_fpr.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_dblp.json --storeplot "./plots/fpr/dblp_fpr.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_dblp_10000.json --storeplot "./plots/fpr/dblp_fpr_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_python.json --storeplot "./plots/fpr/python_fpr.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_python_10000.json --storeplot "./plots/fpr/python_fpr_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_sentiment.json --storeplot "./plots/fpr/sentiment_fpr.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_swissprot.json --storeplot "./plots/fpr/swissprot_fpr.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/fpr/fpr_swissprot_10000.json --storeplot "./plots/fpr/swissprot_fpr_10000.pdf"
echo " --- Done FPR --- "

### Join Times
echo " --- Start Join Times --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_bolzano.json --storeplot "./plots/times/bolzano_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_dblp.json --storeplot "./plots/times/dblp_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_dblp_10000.json --storeplot "./plots/times/dblp_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_python.json --storeplot "./plots/times/python_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_python_10000.json --storeplot "./plots/times/python_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_sentiment.json --storeplot "./plots/times/sentiment_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_swissprot.json --storeplot "./plots/times/swissprot_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/times/times_swissprot_10000.json --storeplot "./plots/times/swissprot_times_10000.pdf"
echo " --- Done Join Times --- "

### Candidate Times
echo " --- Start Candidate Times --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_bolzano.json --storeplot "./plots/cand_times/bolzano_cand_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_dblp.json --storeplot "./plots/cand_times/dblp_cand_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_dblp_10000.json --storeplot "./plots/cand_times/dblp_cand_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_python.json --storeplot "./plots/cand_times/python_cand_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_python_10000.json --storeplot "./plots/cand_times/python_cand_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_sentiment.json --storeplot "./plots/cand_times/sentiment_cand_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_swissprot.json --storeplot "./plots/cand_times/swissprot_cand_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times/cand_times_swissprot_10000.json --storeplot "./plots/cand_times/swissprot_cand_times_10000.pdf"
echo " --- Done Candidate Times --- "

### Candidate Times + Pre Comp Times
echo " --- Start Candidate Times + Pre Comp Times --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_bolzano.json --storeplot "./plots/cand_times_conv/bolzano_cand_conv_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_dblp.json --storeplot "./plots/cand_times_conv/dblp_cand_conv_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_dblp_10000.json --storeplot "./plots/cand_times_conv/dblp_cand_conv_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_python.json --storeplot "./plots/cand_times_conv/python_cand_conv_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_python_10000.json --storeplot "./plots/cand_times_conv/python_cand_conv_times_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_sentiment.json --storeplot "./plots/cand_times_conv/sentiment_cand_conv_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_swissprot.json --storeplot "./plots/cand_times_conv/swissprot_cand_conv_times.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/cand_times_conv/cand_times_conv_swissprot_10000.json --storeplot "./plots/cand_times_conv/swissprot_cand_conv_times_10000.pdf"
echo " --- Done Candidate Times + Pre Comp Times --- "

### Candidates
echo " --- Start Candidates --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_bolzano.json --storeplot "./plots/candidates/bolzano_candidates.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_dblp.json --storeplot "./plots/candidates/dblp_candidates.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_dblp_10000.json --storeplot "./plots/candidates/dblp_candidates_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_python.json --storeplot "./plots/candidates/python_candidates.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_python_10000.json --storeplot "./plots/candidates/python_candidates_10000.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_sentiment.json --storeplot "./plots/candidates/sentiment_candidates.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_swissprot.json --storeplot "./plots/candidates/swissprot_candidates.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/candidates/candidates_swissprot_10000.json --storeplot "./plots/candidates/swissprot_candidates_10000.pdf"
echo " --- Done Candidates --- "

### RSB vs RSC Sentiment
echo " --- Start RSB vs RSC Sentiment --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/candidates_sentiment_rsb.json --storeplot "./plots/rsb_rsc/candidates_sentiment_rsb.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/candidates_sentiment_rsc.json --storeplot "./plots/rsb_rsc/candidates_sentiment_rsc.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/metric_cand_sentiment_rsb.json --storeplot "./plots/rsb_rsc/metric_cand_sentiment_rsb.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/metric_cand_sentiment_rsc.json --storeplot "./plots/rsb_rsc/metric_cand_sentiment_rsc.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/times_nomet_sentiment_rsb.json --storeplot "./plots/rsb_rsc/times_nomet_sentiment_rsb.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/times_nomet_sentiment_rsc.json --storeplot "./plots/rsb_rsc/times_nomet_sentiment_rsc.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/times_sentiment_rsb.json --storeplot "./plots/rsb_rsc/times_sentiment_rsb.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/rsb_rsc/times_sentiment_rsc.json --storeplot "./plots/rsb_rsc/times_sentiment_rsc.pdf"
echo " --- Done RSB vs RSC Sentiment --- "

### Variable dataset sizes DBLP
echo " --- Start Variable dataset sizes DBLP --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/candidates_dblp_t1.json --storeplot "./plots/dataset_size/dblp/candidates_dblp_t1.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/candidates_dblp_t3.json --storeplot "./plots/dataset_size/dblp/candidates_dblp_t3.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t1_rsb_var_k.json --storeplot "./plots/dataset_size/dblp/times_dblp_t1_rsb_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t3_rsb_var_k.json --storeplot "./plots/dataset_size/dblp/times_dblp_t3_rsb_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t1_rsc_var_k.json --storeplot "./plots/dataset_size/dblp/times_dblp_t1_rsc_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t3_rsc_var_k.json --storeplot "./plots/dataset_size/dblp/times_dblp_t3_rsc_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t1.json --storeplot "./plots/dataset_size/dblp/times_dblp_t1.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/dblp/times_dblp_t3.json --storeplot "./plots/dataset_size/dblp/times_dblp_t3.pdf"
echo " --- Done Variable dataset sizes DBLP --- "

### Variable dataset sizes Swissprot
echo " --- Start Variable dataset sizes Swissprot --- "
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/candidates_swissprot_t1.json --storeplot "./plots/dataset_size/swissprot/candidates_swissprot_t1.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/candidates_swissprot_t10.json --storeplot "./plots/dataset_size/swissprot/candidates_swissprot_t10.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t1_rsb_var_k.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t1_rsb_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t10_rsb_var_k.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t10_rsb_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t1_rsc_var_k.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t1_rsc_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t10_rsc_var_k.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t10_rsc_var_k.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t1.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t1.pdf"
python3 plot_experiments_n.py --service ted-exp --config configs_revision/dataset_size/swissprot/times_swissprot_t10.json --storeplot "./plots/dataset_size/swissprot/times_swissprot_t10.pdf"
echo " --- Done Variable dataset sizes Swissprot --- "