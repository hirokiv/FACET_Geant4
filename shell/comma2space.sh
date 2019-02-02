tail -n +11 ntuple_nt_02_t0.csv | tr "," "\t"  > beam_electron_t0.txt
tail -n +11 ntuple_nt_02_t1.csv | tr "," "\t"  > beam_electron_t1.txt
tail -n +11 ntuple_nt_02_t2.csv | tr "," "\t"  > beam_electron_t2.txt
tail -n +11 ntuple_nt_02_t3.csv | tr "," "\t"  > beam_electron_t3.txt
tail -n +11 ntuple_nt_02_t4.csv | tr "," "\t"  > beam_electron_t4.txt
tail -n +11 ntuple_nt_02_t5.csv | tr "," "\t"  > beam_electron_t5.txt
tail -n +11 ntuple_nt_02_t6.csv | tr "," "\t"  > beam_electron_t6.txt
tail -n +11 ntuple_nt_02_t7.csv | tr "," "\t"  > beam_electron_t7.txt

tail -n +11 ntuple_nt_03_t0.csv | tr "," "\t"  > beam_positron_t0.txt
tail -n +11 ntuple_nt_03_t1.csv | tr "," "\t"  > beam_positron_t1.txt
tail -n +11 ntuple_nt_03_t2.csv | tr "," "\t"  > beam_positron_t2.txt
tail -n +11 ntuple_nt_03_t3.csv | tr "," "\t"  > beam_positron_t3.txt
tail -n +11 ntuple_nt_03_t4.csv | tr "," "\t"  > beam_positron_t4.txt
tail -n +11 ntuple_nt_03_t5.csv | tr "," "\t"  > beam_positron_t5.txt
tail -n +11 ntuple_nt_03_t6.csv | tr "," "\t"  > beam_positron_t6.txt
tail -n +11 ntuple_nt_03_t7.csv | tr "," "\t"  > beam_positron_t7.txt

cat beam_electron_t*.txt > beam1.txt
cat beam_positron_t*.txt > beam2.txt

