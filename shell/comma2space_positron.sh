echo "===== Comma2space.sh called ====="  >> process.log

tail -n +11 ntuple_nt_03_t0.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t0.txt
tail -n +11 ntuple_nt_03_t1.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t1.txt
tail -n +11 ntuple_nt_03_t2.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t2.txt
tail -n +11 ntuple_nt_03_t3.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t3.txt
tail -n +11 ntuple_nt_03_t4.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t4.txt
tail -n +11 ntuple_nt_03_t5.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t5.txt
tail -n +11 ntuple_nt_03_t6.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t6.txt
tail -n +11 ntuple_nt_03_t7.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t7.txt
tail -n +11 ntuple_nt_03_t8.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t8.txt
tail -n +11 ntuple_nt_03_t9.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t9.txt
tail -n +11 ntuple_nt_03_t10.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t10.txt
tail -n +11 ntuple_nt_03_t11.csv | sed -e  's/,/\ \ \ /g'  > beam_positron_t11.txt

cat beam_positron_t*.txt > beam_positron.txt

echo ' ' >> process.log
echo ' - #  positrons' >> process.log
echo `wc -l beam_positron.txt`   >> process.log
