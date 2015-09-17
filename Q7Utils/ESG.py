from ctypes import *

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

if len(sys.argv)==2:
  print "write file",sys.argv[1]
  flen = os.stat(sys.argv[1])
  l = min(flen,esg_len)
  off = 0
  f = open(sys.argv[1])
  while l>4096:
    lib.write( tx, f.read(4096), 4096, c_long( off ) )
    l -= 4096
    off += 4096
  if l>0:
    lib.write( tx, f.read(l), l, c_long( off ) )
    
    
  
  

