echo "===== Comma2space.sh called ====="  >> process.log

tail -n +11 ntuple_nt_04_t0.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t0.txt
tail -n +11 ntuple_nt_04_t1.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t1.txt
tail -n +11 ntuple_nt_04_t2.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t2.txt
tail -n +11 ntuple_nt_04_t3.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t3.txt
tail -n +11 ntuple_nt_04_t4.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t4.txt
tail -n +11 ntuple_nt_04_t5.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t5.txt
tail -n +11 ntuple_nt_04_t6.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t6.txt
tail -n +11 ntuple_nt_04_t7.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t7.txt
tail -n +11 ntuple_nt_04_t8.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t8.txt
tail -n +11 ntuple_nt_04_t9.csv | sed -e  's/,/\ \ \ /g'  > beam_prim_ele_t9.txt
tail -n +11 ntuple_nt_04_t10.csv | sed -e  's/,/\ \ \ /g' > beam_prim_ele_t10.txt
tail -n +11 ntuple_nt_04_t11.csv | sed -e  's/,/\ \ \ /g' > beam_prim_ele_t11.txt

cat beam_prim_ele_t*.txt > beam_prim_ele.txt

echo ' ' >> process.log
echo ' - # of primary electrons ' >> process.log
echo `wc -l beam_prim_ele.txt`   >> process.log
