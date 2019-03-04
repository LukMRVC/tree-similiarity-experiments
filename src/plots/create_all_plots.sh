rm -rf plots
mkdir plots
cd plots
mkdir join_times
mkdir cand_times
mkdir candidates
mkdir upperbound
cd ..

### Join Times
echo " --- Start Join Times --- "
python3 plot_experiments.py --service ted-exp --config configs/icde2019/join_times/times_dblp.json --storeplot "./plots/join_times/dblp_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/join_times/times_dblp_size_t6.json --storeplot "./plots/join_times/times_dblp_size_t6.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/join_times/times_python.json --storeplot "./plots/join_times/python_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/join_times/times_sentiment.json --storeplot "./plots/join_times/sentiment_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/join_times/times_swissprot.json --storeplot "./plots/join_times/swissprot_times.pdf"
echo " --- Done Join Times --- "

### Candidate Times
echo " --- Start Candidate Times --- "
python3 plot_experiments.py --service ted-exp --config configs/icde2019/cand_times/cand_times_dblp.json --storeplot "./plots/cand_times/dblp_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/cand_times/cand_times_dblp_size_t6.json --storeplot "./plots/cand_times/dblp_size_t6_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/cand_times/cand_times_python.json --storeplot "./plots/cand_times/python_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/cand_times/cand_times_sentiment.json --storeplot "./plots/cand_times/sentiment_cand_times.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/cand_times/cand_times_swissprot.json --storeplot "./plots/cand_times/swissprot_cand_times.pdf"
echo " --- Done Candidate Times --- "

### Candidates
echo " --- Start Candidates --- "
python3 plot_experiments.py --service ted-exp --config configs/icde2019/candidates/candidates_dblp.json --storeplot "./plots/candidates/dblp_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/candidates/candidates_dblp_size_t6.json --storeplot "./plots/candidates/dblp_size_t6_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/candidates/candidates_python.json --storeplot "./plots/candidates/python_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/candidates/candidates_sentiment.json --storeplot "./plots/candidates/sentiment_candidates.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/candidates/candidates_swissprot.json --storeplot "./plots/candidates/swissprot_candidates.pdf"
echo " --- Done Candidates --- "

### Upperbound
echo " --- Start Candidates --- "
python3 plot_experiments.py --service ted-exp --config configs/icde2019/upperbound/sentiment_runtime.json --storeplot "./plots/upperbound/sentiment_runtime.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/upperbound/sentiment_error.json --storeplot "./plots/upperbound/sentiment_error.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/upperbound/dblp_runtime.json --storeplot "./plots/upperbound/dblp_runtime.pdf"
python3 plot_experiments.py --service ted-exp --config configs/icde2019/upperbound/dblp_error.json --storeplot "./plots/upperbound/dblp_error.pdf"
echo " --- Done Candidates --- "