import matplotlib.ticker as ptick
import matplotlib.pyplot as plt
import struct
#from scipy.io import FortranFile
import numpy as np
import sys
#import pandas as pd

args = ''.join(sys.argv[1:])

##################################################
# FLAGS
##################################################
DNUM = 20
TRAPFLAG  = 0  # if you count only trapped particles or not
##################################################

# Fontsize of figs 
plt.rcParams["font.size"] = 16

# Output analysis results to the file 
# for piping to OSIRIS input
# filename = 'EGS5_ebin_output_dnum' + '%04d' % DNUM + '.txt'
# Limiting the particles counted to be less than 0.1 rad

# for specy_str in {"positron","elec_prm","elec_sec","photon"}:
for specy_str in {args}:
#for specy_str in ["beam_electron", "beam_positron"]:
  
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
  

  ################################################
  # Read out number of particles concerned
  ################################################
  # file type .txt
  filename = specy_str + ".txt"
  chunk = np.genfromtxt(filename, dtype=float, delimiter='', names=None)

  CNT = num_lines = sum(1 for line in open(filename))
 
  # Set bin for every NPB particles
  NPB = int(CNT/DNUM)+1
  
  print "# of particles   :",CNT
  print "medcnt/",DNUM,":",NPB


  ebin = np.zeros(CNT)
  re = np.zeros(CNT)
  rx = np.zeros(CNT)
  ry = np.zeros(CNT)
  rz = np.zeros(CNT)
  ru = np.zeros(CNT)
  rv = np.zeros(CNT)
  rw = np.zeros(CNT)

  for j in range(CNT):
    rx[j] = chunk[j][0]               # pos x in m
    ry[j] = chunk[j][1]               # pos y in m
    ru[j] = chunk[j][2]               # vel x in cos
    rv[j] = chunk[j][3]               # vel y in cos
    rz[j] = chunk[j][4]               # pos ct-z in m
    re[j] = chunk[j][5] #*0.511       # Energy in gamma

 
  ##########################################################################################
  #  Note that we limit angle for analysis here for the sake of analysis. 
  #  If you need entire particles, set angle-> Infinity
  ##########################################################################################
    # Filter only the particles whose angle is less than 0.1 rad
  c_omegae = 3e8/np.sqrt(1.6e-19*1.6e-19*2.5e22/8.85e-12/1.6e-31) # electron skin depth
 
#   if TRAPFLAG:
#     angle = 1./2./np.pi # Angle determined by lambda_p v.s. c/omega_p
#     specy_str = 'trap_' + specy_str
#   else:
#     angle = 10000000000000 # Taking nearly infinite rad
#   
#   filt = np.where( (np.sqrt(ru[:]*ru[:]+rv[:]*rv[:])) < angle)[0]
#   #  print filt
#   CNT_FILT = len(filt)
#   
#   re = re[filt]
#   rx = rx[filt]
#   ry = ry[filt]
#   ru = ru[filt]
#   rv = rv[filt]
  
  # Convert to rad
  xangle = np.arctan(ru)
  yangle = np.arctan(rv)
   #######################################################
# resave
  for shift_um in [0, +150]:
    rz = rz - shift_um*1e-6
    A = np.hstack( (rx.reshape(CNT,1), ry.reshape(CNT,1), ru.reshape(CNT,1), rv.reshape(CNT,1), rz.reshape(CNT,1), re.reshape(CNT,1) ) )
#    print rx
#    print A
    filestr = filename + '_shift_' + str(shift_um) + '_um.txt'
    np.savetxt(filestr, A, fmt="%12.5e  ", delimiter=" ")

  #######################################################
#   fig5 = plt.plot( rz, re , "o")
#   plt.show()
#   filename = specy_str + "_energy.png"
#   fig5.savefig(filename,bbox_inches='tight')

  print '========================='
  print 'Average in X'
  print ' mean   ' + str(np.average(rx)     )   
  print ' sigma  ' + str(np.sqrt(np.var(rx))) 
  print 'Average in Y'
  print ' mean   ' + str(np.average(ry)     )
  print ' sigma  ' + str(np.sqrt(np.var(ry))) 
  print 'Average of U'
  print ' mean   ' + str(np.average(ru)     )
  print ' sigma  ' + str(np.sqrt(np.var(ru))) 
  print 'Average of V'
  print ' mean   ' + str(np.average(rv)     )
  print ' sigma  ' + str(np.sqrt(np.var(rv))) 
  print 'Average of CT-Z'
  print ' mean   ' + str(np.average(rz)     )
  print ' sigma  ' + str(np.sqrt(np.var(rz))) 
  print '========================='


