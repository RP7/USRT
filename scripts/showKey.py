#!/usr/bin/env python  
  
from ctypes import *  
import sys
  
dll = CDLL(sys.argv[1])                 
key = (c_longlong *2)()
item = dll.newFun()
dll.getKey(item,byref(key))
print '%016x%016x' % (key[1],key[0])
          