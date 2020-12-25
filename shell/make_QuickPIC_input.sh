rm -f beam?_temp.txt

echo "Writing the progress to process.log....."

echo "make_QuickPIC.sh" > process.log

# Export CSV file into QuickPIC input
# Only beam1.txt and beam2.txt
echo "======================================" >> process.log
echo "make_QuickPIC.sh call comma2space.sh" >> process.log
echo "- particles numbers will be truncated   " >> process.log 
echo "======================================" >> process.log
echo "make_QuickPIC.sh - Generating beam1 and beam2 " >> process.log

sh ../shell/comma2space_electron.sh
mv beam_electron.txt  beam_electron_temp.txt
cat beam_electron_temp.txt | sed -e '1000001,$d' > beam_electron.txt
rm  beam_electron_temp.txt

sh ../shell/comma2space_positron.sh
mv beam_positron.txt  beam_positron_temp.txt
cat beam_positron_temp.txt | sed -e '1000001,$d' > beam_positron.txt
rm  beam_positron_temp.txt

sh ../shell/comma2space_prim_ele.sh
mv beam_prim_ele.txt  beam_prim_ele_temp.txt
cat beam_prim_ele_temp.txt | sed -e '1000001,$d' > beam_prim_ele.txt
rm  beam_prim_ele_temp.txt


# Shift beam files by 150 um to generate drive beam
echo "======================================" >> process.log
echo "make_QuickPIC.sh call inject_params.py" >> process.log
echo "======================================" >> process.log
python3 ../python/inject_params.py "beam_electron" >> process.log
python3 ../python/inject_params.py "beam_positron" >> process.log


# Register shifted beam file as beam3 and beam 4
cat beam_electron.txt_shift_150*  > beam1.txt
cat beam_positron.txt_shift_150*  > beam2.txt
echo "======================================" >> process.log
echo "beam1 & beam2 created"                  >> process.log
echo "======================================" >> process.log

cat beam_electron.txt_shift_0*  > beam3.txt
cat beam_positron.txt_shift_0*  > beam4.txt
echo "======================================" >> process.log
echo "beam3 & beam4 created"                  >> process.log
echo "======================================" >> process.log

# # Add blank lines ( Specification of QuickPIC )
# echo " " >> beam1.txt
# echo " " >> beam2.txt
# echo " " >> beam3.txt
# echo " " >> beam4.txt
mv beam1.txt beam1_temp.txt
mv beam2.txt beam2_temp.txt
mv beam3.txt beam3_temp.txt
mv beam4.txt beam4_temp.txt
sed -e 's/e/E/g' beam1_temp.txt > beam1.txt
sed -e 's/e/E/g' beam2_temp.txt > beam2.txt
sed -e 's/e/E/g' beam3_temp.txt > beam3.txt
sed -e 's/e/E/g' beam4_temp.txt > beam4.txt

# Limit number of particles
mv beam2.txt beam2_temp.txt
mv beam3.txt beam3_temp.txt

cat beam2_temp.txt | sed -e '500001,$d' > beam2.txt
cat beam3_temp.txt | sed -e '500001,$d' > beam3.txt


rm  -f beam?_temp.txt
for i in `seq 0 18`
do
  for j in prim_ele positron electron
  do
    filename="beam_${j}_t${i}.txt"
    if [ -e $filename ]; then
      rm -f $filename
    fi
  done
done


