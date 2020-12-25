echo "===== Comma2space_positron.sh called ====="  >> process.log


for i in `seq 0 18`
do
  filename="ntuple_nt_03_t$i.csv"
  if [ -e $filename ]; then
    tail -n +11 $filename | sed -e  's/,/\ \ \ /g'  > "beam_positron_t$i.txt"
  fi
done

cat beam_positron_t*.txt > beam_positron.txt

echo ' - #  positrons' >> process.log
echo `wc -l beam_positron.txt`   >> process.log
echo ' ' >> process.log
