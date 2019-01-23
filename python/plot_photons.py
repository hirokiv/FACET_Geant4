#!/usr/bin/python
# -*- Coding: utf-8 -*-


import matplotlib.ticker as ptick
import matplotlib.pyplot as plt
import struct
#from scipy.io import FortranFile
import numpy as np
#import pandas as pd


##################################################
# FLAGS
##################################################
DNUM = 20
TRAPFLAG  = 0  # if you count only trapped particles or not
##################################################
INCIDENT = 13000000
#################################################

# Fontsize of figs 
plt.rcParams["font.size"] = 16

# Output analysis results to the file 
# for piping to OSIRIS input
filename = 'EGS5_ebin_output_dnum' + '%04d' % DNUM + '.txt'
# Limiting the particles counted to be less than 0.1 rad 

# for specy_str in {"positron","elec_prm","elec_sec","photon"}:
for specy_str in ["photon"]:
  
  ##############################################
  # File ID
  ##############################################
  # fid = open(filename, 'w')
  # fid.writelines('EBIN[MeV]; X[um]; U[mrad]; Y[um]; V[mrad]; Ratio per incident electrons')
  # fid.write('\n')
  # fid.writelines( '%04d' % DNUM )
  # fid.write('\n\n')
  
  # fid.writelines(specy_str)
  # fid.write('\n')
  
  
  
  if specy_str in "electron":
    spidx = 0
  elif specy_str in "photon":
    spidx = 1
  elif specy_str in "positron":
    spidx = 2
  elif specy_str in "elec_prm":
    spidx = 3
  elif specy_str in "elec_sec":
    spidx = 4
  
  ################################################
  # Read out number of particles concerned
  ################################################
  
  # file type .txt
  filename = "d_" + specy_str + ".txt"
  chunk = np.genfromtxt(filename, dtype=float, delimiter='', names=None)

  CNT = chunk.shape[0]


  ebin = np.zeros(CNT)
  re = np.zeros(CNT)
  rx = np.zeros(CNT)
  ry = np.zeros(CNT)
  rz = np.zeros(CNT)
  ru = np.zeros(CNT)
  rv = np.zeros(CNT)
  rw = np.zeros(CNT)
  
  for j in range(CNT):
    re[j] = chunk[j][0]        # Energy in MeV
    rx[j] = chunk[j][1]        # pos x in cm
    ry[j] = chunk[j][2]        # pos y in cm
    rz[j] = chunk[j][3]        # pos z in cm
    ru[j] = chunk[j][4]        # vel x in cos
    rv[j] = chunk[j][5]        # vel y in cos
    rw[j] = chunk[j][6]        # vel z in cos


  
  #################################################### 
  #         Sort to obey the order of energy
  #################################################### 
 
  # Convert to rad
  xangle = np.arctan(ru/rw)
  yangle = np.arctan(rv/rw)
  
  # Spectrum arrays
  spec = np.zeros(int(max(ebin)))
  avsp = np.zeros(int(max(ebin)))
  spec2s = np.zeros(int(max(ebin)))
  sigsp = np.zeros(int(max(ebin)))
  
  # Emittance arrays
  xgemit = np.zeros(int(max(ebin)))
  ygemit = np.zeros(int(max(ebin)))
  
  xsig = np.zeros(int(max(ebin)))
  usig = np.zeros(int(max(ebin)))
  ysig = np.zeros(int(max(ebin)))
  vsig = np.zeros(int(max(ebin)))
  
  eidx = np.zeros(int(DNUM))
  
  

  ######################################################
  # Replicate Nature photonics paper (Albert_Giant_2018)
  # Electromagnetic interaction not considered in EGS5
  ######################################################
  
   
  # Set bin for every NPB particles
  lin_00001 = np.linspace(0.1,1,30)
  lin_00010 = np.linspace(1.,10.,30)
  lin_00100 = np.linspace(10.,100.,30)
  lin_01000 = np.linspace(100.,1000.,30)
  lin_10000 = np.linspace(1000.,3000.,30)
  ebin_label = np.concatenate((lin_00001, lin_00010, lin_00100, lin_01000, lin_10000), axis=1)

  ebin_len = len(ebin_label)
  photon_spec = np.zeros(ebin_len)
  t_pass= 1/3e10 # 1[cm]/3e10[cm/s] 

  fig7 = plt.figure(figsize=(7,8))
  axL = fig7.add_subplot(1,1,1)

  N_label = [3.e18, 3.e19, 1.e20, 2.e20, 6.e20]
  
  for N in N_label:
    for i in range(ebin_len):
        # Actually taking 
      idx = ( re > (ebin_label[i]-ebin_label[i]/1000.) ) * ( re < (ebin_label[i]+ebin_label[i]/1000.) )
      xvar = np.sqrt(np.var(xangle[idx])) * 1000.  # mrad
      yvar = np.sqrt(np.var(yangle[idx])) * 1000.  # mrad

#      print np.sum(idx)
      photon_spec[i] = np.sum(idx)
      photon_spec[i] = photon_spec[i] / 1e2 / t_pass # Convert from cm-3 to mm-2 s-1
      photon_spec[i] = photon_spec[i] / INCIDENT * N   # Consider particle numbers in cubic cm
      photon_spec[i] = photon_spec[i] / xvar / yvar   # change unit -> mrad-2
      photon_spec[i] = photon_spec[i] / 4. /np.pi/np.pi # Brilliance by definition
      photon_spec[i] = photon_spec[i] / 2  # compensation to be 0.1%
  
#    print photon_spec 
    axL.loglog(ebin_label,photon_spec, linewidth=2, label=str(N)+' cm^{-3}')
  # fig.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  #axL.set_yscale("log")
#  axL.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
#  axL.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
#  axL.set_xticks([0.00,2500,5000,7500,10000])
  axL.set_ylabel(r"Brilliance (photons s$^{-1}$ mm$^{-2}$ mrad$^{-2}$ per 0.1 % BW)")
  axL.set_xlabel("Energy [MeV]")
  axL.set_xlim([0.2, 3000])
  axL.set_ylim([1e20, 1e26])
  axL.grid()
  #  Make smaller CDF plot 
  filename= specy_str + '_Brilliance.png'
  fig7.savefig(filename,bbox_inches='tight')
  
#  
#  ## Output to file
#  def d2s(data):
#  #  panda_data = pd.DataFrame(data)
#  #  panda_data = panda_data.round(4)
#    data = np.array(data)
#    data_str = ",".join(map(str, data))
#    return data_str.replace('[','').replace(']','')
#  
#  fid.write(d2s(eidx))
#  fid.write('\n')
#  fid.write(d2s(xsig))
#  fid.write('\n')
#  fid.write(d2s(usig))
#  fid.write('\n')
#  fid.write(d2s(ysig))
#  fid.write('\n')
#  fid.write(d2s(vsig))
#  fid.write('\n')
#  fid.write(d2s(p_bin))
#  fid.write('\n')
#  fid.write('\n')
#  
#  fid.close()
