sh ../shell/comma2space_prim_ele.sh
mv beam_prim_ele.txt  beam_prim_ele_temp.txt
cat beam_prim_ele_temp.txt | sed -e '1000001,$d' > beam_prim_ele.txt
rm  beam_prim_ele_temp.txt

