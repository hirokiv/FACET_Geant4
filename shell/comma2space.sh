tail -n +11 ntuple_nt_02_t0.csv | tr "," "\t"  > beam_electron_t0.txt
tail -n +11 ntuple_nt_02_t1.csv | tr "," "\t"  > beam_electron_t1.txt
tail -n +11 ntuple_nt_03_t0.csv | tr "," "\t"  > beam_positron_t0.txt
tail -n +11 ntuple_nt_03_t1.csv | tr "," "\t"  > beam_positron_t1.txt

cat beam_electron_t*.txt > beam1.txt
cat beam_positron_t*.txt > beam2.txt

