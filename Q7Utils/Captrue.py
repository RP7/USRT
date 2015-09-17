from ctypes import *
import os
import sys
import time

class capture:
  def __init__(self):
    dirname,filename = os.path.split(__file__)
    workingDir=dirname + '/../work'
    fn = workingDir+'/libQ7.so'
    self.lib = CDLL(fn)

    self.rx = self.lib.attachQ7Mem('rx_udp.d')

    self.lib.dumpQ7Mem( self.rx )
    self.cap_size = self.lib.mSize( self.rx )
    print "Capture Mem Size",self.cap_size
    t_offset = (c_long*1)()
    self.lib.offset( self.rx, t_offset )
    self.start = long(t_offset[0]) 
    self.blk = 4096
    
  def _off( self ):
    t_offset = (c_long*1)()
    self.lib.offset( self.rx, t_offset )
    return long(t_offset[0]) 
    
  def getOff( self, off ):
    pos = long(self.start+off)
    cnt = 0
    while self._off()<pos+self.blk:
      time.sleep(0.001)
      cnt += 1
      if cnt>1000:
        print "time out, no packet recv"
        break
    return self.lib.getAddr( self.rx, c_long( off ) )
    
  def read( self, buf, l, off ):
    pos = long(self.start+off)
    cnt = 0
    while self._off()<pos+self.blk:
      time.sleep(0.001)
      cnt += 1
      if cnt>1000:
        print "time out, no packet recv"
        break
    self.lib.Q7read( self.rx, buf, l, pos ) 
        
if __name__ == '__main__':  
  if len(sys.argv)==3:
    l = int(sys.argv[2])
    rx = capture()
    print "write file",sys.argv[1],'length = ',l
    off = 0
    f = open(sys.argv[1],'wb')
    buf = (c_char*rx.blk)()
    while l>=rx.blk:
      rx.read(buf,rx.blk,off)
      f.write(buf)
      l -= rx.blk
      off += rx.blk
    if l>0:
      rx.read(buf,l,off)
      f.write(buf)
    f.close()
     
    
  
  

