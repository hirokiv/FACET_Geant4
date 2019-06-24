echo "===== Comma2space.sh called ====="  >> process.log

tail -n +11 ntuple_nt_02_t0.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t0.txt
tail -n +11 ntuple_nt_02_t1.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t1.txt
tail -n +11 ntuple_nt_02_t2.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t2.txt
tail -n +11 ntuple_nt_02_t3.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t3.txt
tail -n +11 ntuple_nt_02_t4.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t4.txt
tail -n +11 ntuple_nt_02_t5.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t5.txt
tail -n +11 ntuple_nt_02_t6.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t6.txt
tail -n +11 ntuple_nt_02_t7.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t7.txt
tail -n +11 ntuple_nt_02_t8.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t8.txt
tail -n +11 ntuple_nt_02_t9.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t9.txt
tail -n +11 ntuple_nt_02_t10.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t10.txt
tail -n +11 ntuple_nt_02_t11.csv | sed -e  's/,/\ \ \ /g'  > beam_electron_t11.txt


cat beam_electron_t*.txt > beam_electron.txt

echo ' ' >> process.log
echo ' - # of entire electrons' >> process.log
echo `wc -l beam_electron.txt`   >> process.log


cat beam_electron_t0.txt > beam_electron.txt
cat beam_electron_t1.txt >> beam_electron.txt
cat beam_electron_t2.txt >> beam_electron.txt
