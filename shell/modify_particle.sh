#!/bin/sh
# Merge particle files into one file 
# take a million lines from the top

# delete empty
find . -empty -delete



date > modify_particle.log
echo '# of files of electron/positron' >> modify_particle.log
ls -1 beam_electron.txt | wc -l | tr '\012' ' '  >> modify_particle.log
ls -1 beam_positron.txt | wc -l >> modify_particle.log

##################################################################
# Delete previous files
##################################################################

rm beam_d_electron.txt
rm beam_d_elec_sec.txt
rm beam_d_elec_prm.txt
rm beam_d_positron.txt
rm beam_d_photon.txt

rm beam_t_electron.txt
rm beam_t_elec_sec.txt
rm beam_t_elec_prm.txt
rm beam_t_positron.txt
rm beam_t_photon.txt

##################################################################
#  Drive beam
##################################################################

echo 'Process drive electron' >> modify_particle.log
cat d_electron* > d_electron_temp.txt
cat d_electron_temp.txt | sed -e '1000001,$d' > beam_d_electron.txt
cat d_electron_temp.txt | wc -l >> modify_particle.log
rm d_electron_temp.txt

echo 'Process drive positron' >> modify_particle.log
cat d_positron* > d_positron_temp.txt
cat d_positron_temp.txt | sed -e '1000001,$d' > beam_d_positron.txt
cat d_positron_temp.txt | wc -l >> modify_particle.log
rm d_positron_temp.txt

echo 'Process drive photon' >> modify_particle.log
cat d_photon__* > d_photon_temp.txt
cat d_photon_temp.txt   | sed -e '1000001,$d' > beam_d_photon.txt
cat d_photon_temp.txt | wc -l >> modify_particle.log
rm d_photon_temp.txt

echo 'Process drive primary electron' >> modify_particle.log
cat d_elec_prm* > d_elec_prm_temp.txt
cat d_elec_prm_temp.txt | sed -e '1000001,$d' > beam_d_elec_prm.txt
cat d_elec_prm_temp.txt | wc -l >> modify_particle.log
rm d_elec_prm_temp.txt

echo 'Process drive secondary electron' >> modify_particle.log
cat d_elec_sec* > d_elec_sec_temp.txt
cat d_elec_sec_temp.txt | sed -e '1000001,$d' > beam_d_elec_sec.txt
cat d_elec_sec_temp.txt | wc -l >> modify_particle.log
rm d_elec_sec_temp.txt



##################################################################
#  Trailing beam
##################################################################
echo 'Process trailing electron' >> modify_particle.log
cat t_electron* > t_electron_temp.txt
cat t_electron_temp.txt | sed -e '1000001,$d' > beam_t_electron.txt
cat t_electron_temp.txt | wc -l >> modify_particle.log
rm t_electron_temp.txt

echo 'Process trailing positron' >> modify_particle.log
cat t_positron* > t_positron_temp.txt
cat t_positron_temp.txt | sed -e '1000001,$d' > beam_t_positron.txt
cat t_positron_temp.txt | wc -l >> modify_particle.log
rm t_positron_temp.txt

echo 'Process trailing photon' >> modify_particle.log
cat t_photon__* > t_photon_temp.txt
cat t_photon_temp.txt   | sed -e '1000001,$d' > beam_t_photon.txt
cat t_photon_temp.txt | wc -l >> modify_particle.log
rm t_photon_temp.txt

echo 'Process trailing primary electron' >> modify_particle.log
cat t_elec_prm* > t_elec_prm_temp.txt
cat t_elec_prm_temp.txt | sed -e '1000001,$d' > beam_t_elec_prm.txt
cat t_elec_prm_temp.txt | wc -l >> modify_particle.log
rm t_elec_prm_temp.txt

echo 'Process trailing secondary electron' >> modify_particle.log
cat t_elec_sec* > t_elec_sec_temp.txt
cat t_elec_sec_temp.txt | sed -e '1000001,$d' > beam_t_elec_sec.txt
cat t_elec_sec_temp.txt | wc -l >> modify_particle.log
rm t_elec_sec_temp.txt


