echo "===== Comma2space_prim_ele.sh called ====="  >> process.log

for i in `seq 0 18`
do
  filename="ntuple_nt_04_t$i.csv"
  if [ -e $filename ]; then
    tail -n +11 $filename | sed -e  's/,/\ \ \ /g'  > "beam_prim_ele_t$i.txt"
  fi
done


cat beam_prim_ele_t*.txt > beam_prim_ele.txt

echo ' - # of primary electrons ' >> process.log
echo `wc -l beam_prim_ele.txt`   >> process.log
echo ' ' >> process.log
