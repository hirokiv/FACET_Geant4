#!/bin/sh


export FACET_GEANT=/DataStorage/Geant4/FACET_gamma

rm -f beam2.txt
rm -f beam4.txt
rm -f beam2_temp.txt
rm -f beam4_temp.txt

echo "make_QuickPIC.sh" > process.log

# Export CSV file into QuickPIC input
# Only beam1.txt and beam2.txt
echo "======================================" >> process.log
echo "make_QuickPIC.sh call comma2space.sh" >> process.log
echo "- particles numbers will be truncated   " >> process.log 
echo "======================================" >> process.log
echo "make_QuickPIC.sh - Generating beam1 and beam2 " >> process.log
sh $FACET_GEANT/shell/comma2space_positron.sh
mv beam_positron.txt  beam_positron_temp.txt
cat beam_positron_temp.txt | sed -e '2000001,$d' > beam_positron.txt
rm  beam_positron_temp.txt

python $FACET_GEANT/python/inject_params.py "beam_positron" >> process.log


# Register shifted beam file as beam3 and beam 4
cat beam_positron.txt_shift_150*  > beam2.txt
cat beam_positron.txt_shift_0*  > beam4.txt
echo "======================================" >> process.log
echo " beam2 created"                  >> process.log
echo "======================================" >> process.log
# # Add blank lines ( Specification of QuickPIC )
# echo " " >> beam1.txt
# echo " " >> beam2.txt
# echo " " >> beam3.txt
# echo " " >> beam4.txt
mv beam2.txt beam2_temp.txt
mv beam4.txt beam4_temp.txt
sed -e 's/e/E/g' beam2_temp.txt > beam2.txt
sed -e 's/e/E/g' beam4_temp.txt > beam4.txt

# Limit number of particles
mv beam2.txt beam2_temp.txt
cat beam2_temp.txt | sed -e '500001,$d' > beam2.txt


rm  -f beam?_temp.txt

