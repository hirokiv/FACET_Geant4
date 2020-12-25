echo "===== Comma2space_electron.sh called ====="  >> process.log

for i in `seq 0 18`
do
  filename="ntuple_nt_02_t$i.csv"
  if [ -e $filename ]; then
    tail -n +11 $filename | sed -e  's/,/\ \ \ /g'  > "beam_electron_t$i.txt"
  fi
done

cat beam_electron_t*.txt > beam_electron.txt

echo ' - # of entire electrons' >> process.log
echo `wc -l beam_electron.txt`   >> process.log
echo ' ' >> process.log

# Regenerate beam_electron.txt if the truncation process takes for a long time
#  cat beam_electron_t0.txt > beam_electron.txt
#  cat beam_electron_t1.txt >> beam_electron.txt
#  cat beam_electron_t2.txt >> beam_electron.txt
