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
INCIDENT_DRI = 12000000./11846777. * 1000000.
INCIDENT_TRA = 27000000./1170586.  * 1000000.
#################################################

# Fontsize of figs 
plt.rcParams["font.size"] = 16

# Output analysis results to the file 
# for piping to OSIRIS input
filename = 'EGS5_ebin_output_dnum' + '%04d' % DNUM + '.txt'
# Limiting the particles counted to be less than 0.1 rad

# for specy_str in {"positron","elec_prm","elec_sec","photon"}:
for specy_str in ["gamma___"]:
  
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
    ncount = INCIDENT_DRI
  elif specy_str in "gamma___":
    spidx = 1
  elif specy_str in "positron":
    spidx = 2
    ncount = INCIDENT_TRA
  elif specy_str in "elec_prm":
    spidx = 3
  elif specy_str in "elec_sec":
    spidx = 4

  ################################################
  # Read out number of particles concerned
  ################################################
  # file type .txt
  filename = "beam_" + specy_str + ".txt"
  chunk = np.genfromtxt(filename, dtype=float, delimiter='', names=None)

  CNT = num_lines = sum(1 for line in open(filename))
 
  # Set bin for every NPB particles
  NPB = int(CNT/DNUM)+1
  
  print "medcnt   :",CNT
  print "medcnt/",DNUM,":",NPB
  ################################################
  # Read out particles data, and store to 
  # the individual variables
  ################################################
#    # file type binary
#      dt = np.dtype([head, ("re","<d"), ("rx","<d"), ("ry","<d"), ("rz","<d"), ("ru","<d"), ("rv","<d"), ("rw","<d"), ("rt","<d"), tail])
#      filename = "bin_" + specy_str + ".dat"
#  
#    fd = open(filename,"r")
#    
#    chunk = np.fromfile(fd, dtype=dt, count=CNT)
#    print "============================================="
#    print "Example output to check if file read properly"
#    for i in [6]:
#      print "re:",chunk[i]["re"]
#      print "rx:",chunk[i]["rx"]
#      print "ry:",chunk[i]["ry"]
#      print "rz:",chunk[i]["rz"]
#      print "ru:",chunk[i]["ru"]
#      print "rv:",chunk[i]["rv"]
#      print "rw:",chunk[i]["rw"]
#      print "time:",chunk[i]["rt"]
#    print "============================================="
#  # arr = chunk[0]["arr"].reshape((5,1),order="F")


  ebin = np.zeros(CNT)
  re = np.zeros(CNT)
  rx = np.zeros(CNT)
  ry = np.zeros(CNT)
  rz = np.zeros(CNT)
  ru = np.zeros(CNT)
  rv = np.zeros(CNT)
  rw = np.zeros(CNT)

  for j in range(CNT):
#    re[j] = chunk[j]["re"]        # Energy in MeV
#    rx[j] = chunk[j]["rx"]        # pos x in cm
#    ry[j] = chunk[j]["ry"]        # pos y in cm
#    rz[j] = chunk[j]["rz"]        # pos z in cm
#    ru[j] = chunk[j]["ru"]        # vel x in cos
#    rv[j] = chunk[j]["rv"]        # vel y in cos
#    rw[j] = chunk[j]["rw"]        # vel z in cos
    rx[j] = chunk[j][0]*100        # pos x in cm
    ry[j] = chunk[j][1]*100        # pos y in cm
    ru[j] = chunk[j][2]        # vel x in cos
    rv[j] = chunk[j][3]        # vel y in cos
    rz[j] = chunk[j][4]*100   # pos ct-z in cm
    #    rw[j] = chunk[j][6]        # vel z in cos
    re[j] = (chunk[j][5])*0.511        # Energy in MeV

  #################################################### 
  #         Sort to obey the order of energy
  #################################################### 
  l = np.array([re,rx,ry,ru,rv],dtype='float')
  l= l[:, l[0,:].argsort()]
  #  print l[0, 0-6:-1]
  
  # Set bin for every NPB particles
  ebin_label = np.zeros(int(round(CNT/NPB)+2))
  for j in range(int(round(CNT/NPB)+2)):
    temp = j*NPB
    if temp >= CNT:
      temp = -1
    ebin_label[j] = l[0,temp]
  
  print ebin_label
  
  for j in range(CNT):
    # ebin[j] = round(re[j]) # Energy bin
    ebin[j] = sum(l[0,j] >= ebin_label) - 1 # Energy bin
  
  print ebin
  
  # Check if ebin is stored properly
  # this should be rectangular
  fig1 = plt.figure(figsize=(8,6))
  ax = fig1.add_subplot(1,1,1)
  ax.hist(ebin)
  filename =  specy_str + "_ebin.png"
  fig1.savefig(filename,bbox_inches='tight')
  
  l = np.vstack((ebin,l))
  
  # Filter only the particles whose angle is less than 0.1 rad
  
  c_omegae = 3e8/np.sqrt(1.6e-19*1.6e-19*2.5e22/8.85e-12/1.6e-31) # electron skin depth
  
  ##########################################################################################
  #  Note that we limit angle for analysis here for the sake of analysis. 
  #  If you need entire particles, set angle-> Infinity
  ##########################################################################################
  if TRAPFLAG:
    angle = 1./2./np.pi # Angle determined by lambda_p v.s. c/omega_p
    specy_str = 'trap_' + specy_str
  else:
    angle = 10000000000000 # Taking nearly infinite rad
  
  filt = np.where( (np.sqrt(l[3,:]*l[3,:]+l[4,:]*l[4,:])) < angle)[0]
  #  print filt
  CNT_FILT = len(filt)
  
  ebin = l[0,filt]
  re = l[1,filt]
  rx = l[2,filt]
  ry = l[3,filt]
  ru = l[4,filt]
  rv = l[5,filt]
  # re = l[6,filt]


  # Convert to rad
  xangle = np.arctan(ru)
  yangle = np.arctan(rv)
  
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
  
  N = int(max(ebin))
  spec = np.zeros(N)
  ediff = ( ebin_label[1:] - ebin_label[0:-1] ) 
  for k in range(int(max(ebin))):
    DELTA_EBIN = ediff[k]
    idx = (ebin==k) # ( ebin_label[k] <= re ) * ( re < ebin_label[k+1] )# Boolean
    CNT_K = len(ebin[idx]) # of particles in each k
    if CNT_K == 0:
        CNT_K = 1
    eidx[k] = sum(re[idx])/CNT_K
 
    ## Spectrum data
    spec[k] = CNT_K # Number of particles stored in a bin
    avsp[k] = spec[k]/ncount
    spec2s[k] = spec[k]*spec[k]/ncount
    sigsp[k] = np.sqrt((spec2s[k]-avsp[k]*avsp[k])/ncount)
    avsp[k] = avsp[k]/DELTA_EBIN # per MeV
    sigsp[k] = sigsp[k]/DELTA_EBIN # per MeV
    # print temp
  
    ## x angle emittance
    x2 = np.sum(rx[idx]*rx[idx])/CNT_K
    u2 = np.sum(xangle[idx]*xangle[idx])/CNT_K
    xu = np.sum(rx[idx]*xangle[idx])/CNT_K
    ## y angle emittance
    y2 = np.sum(ry[idx]*ry[idx])/CNT_K
    v2 = np.sum(yangle[idx]*yangle[idx])/CNT_K
    yv = np.sum(ry[idx]*yangle[idx])/CNT_K
    # Convert the unit of geometric emitt. from cm-rad to mm-mrad
    xgemit[k] = np.sqrt(x2*u2-xu*xu)*10000 # mm-mrad 
    ygemit[k] = np.sqrt(y2*v2-yv*yv)*10000 # mm-mrad 
    xsig[k] = np.sqrt(x2)*10000 # um
    usig[k] = np.sqrt(u2)*1000 # mrad
    ysig[k] = np.sqrt(y2)*10000 # um
    vsig[k] = np.sqrt(v2)*1000 # mrad
  
  print 'avsp'
  print avsp
  print 'ebin_label'
  print ebin_label
  
  print ('x_spot',  np.sqrt( x2 ) )
  print ('y_spot',  np.sqrt( y2 ) )
  
  
  #######################################################
  # Show plot of energy dependence
  #######################################################
  #   eidx = range(DELTAE,(N+1)*DELTAE,DELTAE) - DELTAE/2
  #   eidx = ( ebin_label[1:] + ebin_label[0:-1] ) /2
  #   enemax = 10000 # Max energy to show (MeV)
  #   eneidx = enemax/DELTAE
  
  # Spectrum probability per MeV v.s.energy
  fig2 = plt.figure(figsize=(7,7))
  ax = fig2.add_subplot(1,1,1)
  #  fig2.subplots_adjust(left=0.2, bottom=0.15, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  ax.plot(eidx/1000,avsp*1000)
  ax.set_ylabel('Total ' + specy_str + ' ratio per GeV \n Total '+specy_str+' # is ' + str(CNT_FILT))
  ax.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  ax.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  ax.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  ax.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  ax.set_xlabel('Energy [GeV]')
  ax.grid()
  ax.set_xlim([0, 10])
  filename =  specy_str + "_avspec.png"
  fig2.savefig(filename,bbox_inches='tight')
  
  ####################################################
  # Generate CDF
  ####################################################
  
  # def calc_cdf(n):
  #   return n*DELTAE
  
  p_bin = avsp*ediff # probability per bin
  cdf = np.cumsum(p_bin)
  # cdf = cdf*ediff
  # cdf = map(calc_cdf, cdf)
  
  fig3 = plt.figure(figsize=(7,5))
  axL = fig3.add_subplot(1,1,1)
  # fig.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  axL.plot(eidx/1000,cdf, linewidth=2)
  #axL.set_yscale("log")
  axL.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  axL.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  axL.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  axL.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axL.set_xticks(np.array([0.00,2500,5000,7500,10000])/1000)
  axL.set_ylabel("CDF per # of the intial electron")
  axL.set_xlabel("Energy [GeV]")
  axL.grid()
  #  Make smaller CDF plot 
  filename= specy_str + '_energycdf.png'
  fig3.savefig(filename,bbox_inches='tight')
  
  
  #  IDX = int(round(2000/DELTAE)) # Show up to 2000 MeV
  #  IDX = sum(eidx < 2000)
  fig4 = plt.figure(figsize=(4,3))
  ax = fig4.add_subplot(1,1,1)
  #  fig.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  ax.plot(eidx,cdf, linewidth=2)
  ax.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  ax.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  ax.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  ax.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  ax.set_xticks(np.array([0.000,500,1000,1500,2000,2500])/1000)
  ax.set_xlim(np.array([0, 2800])/1000)
  ax.set_xlabel("Energy [GeV]")
  ax.grid()
  filename= specy_str + '_energycdf_smallrange.png'
  fig4.savefig(filename,bbox_inches='tight')
  
  
  #######################################################
  # Emittance per MeV v.s.energy
  #######################################################
  fig5 = plt.figure(figsize=(8,5))
  axL1 = fig5.add_subplot(1,2,1)
  #  fig3.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  axL1.plot(eidx[0:-1],xgemit[0:-1])
  axL1.set_yscale("log")
  axL1.set_ylim([0.001, 1])
  axL1.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axL1.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axL1.grid()
  #  axL1.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axL1.ticklabel_format(style='sci',axis='y',scilimits=(0,0))
  axL1.set_ylabel('Left : X, Right : Y \n RMS Geometric emittance [mm-mrad]')
  axL1.set_xlabel('Energy [GeV]')
  #filename =  specy_str + "_energy_emittance1.png"
  #axL1.savefig(filename,bbox_inches='tight')
  
  axL2 = fig5.add_subplot(1,2,2)
  # axL2 = plt.figure(figsize=(5,5))
  #  fig3.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  axL2.plot(eidx[0:-1]/1000,ygemit[0:-1])
  axL2.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axL2.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axL2.set_yscale("log")
  axL2.set_ylim([0.001, 1])
  #  axL2.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axL2.ticklabel_format(style='sci',axis='y',scilimits=(0,0))
  axL2.set_xlabel('Energy [GeV]')
  axL2.grid()
  
  filename =  specy_str + "_energy_emittance.png"
  fig5.savefig(filename,bbox_inches='tight')
  
  np.savetxt(specy_str+'xgemit.csv',xgemit,delimiter=',')
  np.savetxt(specy_str+'ygemit.csv',ygemit,delimiter=',')
  np.savetxt(specy_str+'eidx.csv',eidx,delimiter=',')
  np.savetxt(specy_str+'avsp.csv',avsp,delimiter=',')
  np.savetxt(specy_str+'xsig.csv',xsig,delimiter=',')
  np.savetxt(specy_str+'ysig.csv',ysig,delimiter=',')
  np.savetxt(specy_str+'usig.csv',usig,delimiter=',')
  np.savetxt(specy_str+'vsig.csv',vsig,delimiter=',')
  
  #######################################################
  # Angle per MeV v.s.energy
  #######################################################
  fig6 = plt.figure(figsize=(9,8))
  axL1 = fig6.add_subplot(2,2,1)
  #  fig4.subplots_adjust(left=0.2, bottom=0.2, right=1.5, top=0.95, wspace=0.15, hspace=0.15)
  axL1.plot(eidx,xsig)
  axL1.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axL1.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axL1.set_yscale("log")
  axL1.set_ylim([1, 100])
  axL1.grid()
  #  axL1.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axL1.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  axL1.set_ylabel('Left : X, Right : Y \n RMS spotsize [um]')
  
  axR1 = fig6.add_subplot(2,2,2)
  axR1.plot(eidx,ysig)
  axR1.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axR1.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axR1.set_yscale("log")
  axR1.set_ylim([1, 100])
  #  axR1.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axR1.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  axR1.grid()
  
  axL2 = fig6.add_subplot(2,2,3)
  axL2.plot(eidx/1000,usig)
  axL2.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axL2.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axL2.set_yscale("log")
  axL2.set_ylim([0.1, 100])
  axL2.set_ylabel('Left : X, Right : Y \n RMS angle [mrad]')
  #  axL2.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axL2.ticklabel_format(style="sci",  axis="y",scilimits=(0,0))
  axL2.set_xlabel('Energy [GeV]')
  axL2.grid()
  
  axR2 = fig6.add_subplot(2,2,4)
  axR2.plot(eidx/1000,vsig)
  axR2.xaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True))
  axR2.ticklabel_format(style="sci",  axis="x",scilimits=(0,0))
  axR2.set_yscale("log")
  axR2.set_ylim([0.1, 100])
  #  axR2.yaxis.set_major_formatter(ptick.ScalarFormatter(useMathText=True)) 
  #  axR2.ticklabel_format(style="sci",axis="y",scilimits=(0,0))
  axR2.set_xlabel('Energy [GeV]')
  axR2.grid()
  
  filename = specy_str + "_xyuv_sigma6.png"
  fig6.savefig(filename,bbox_inches='tight')
 

  
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

