from ctypes import *
import numpy as np

#workingDir='/home/zhaom/works/USRT/work'
import os
dirname,filename = os.path.split(__file__)

workingDir=dirname + '/../work'

import sys
fn = workingDir+'/libQ7.so'
lib = CDLL(fn)

tx = lib.attachQ7Mem('tx_udp.d')

lib.dumpQ7Mem( tx )
esg_size = lib.mSize( tx )
print "ESG Mem Size",esg_size
cossin = np.zeros(1024,dtype='complex')

cossin = np.exp( -1j*2.*np.pi*np.arange(1024)/1024.*64. )*1024.
buf = (c_short*2048)()
for i in range(1024):
  buf[2*i] = int(cossin[i].real)
  buf[2*i+1] = int(cossin[i].imag)

off = 0
l = esg_size

while l>=4096:
    lib.Q7write( tx, addressof(buf), 4096, c_long( off ) )
    l -= 4096
    off += 4096
    
    
  
  

