#!/usr/bin/env python  
from ctypes import *  
import sys
from os import path

def getKey( obj ):
	key = (c_longlong*2)()
	obj['obj'].getKey(obj['item'],byref(key))
	ret = '%016x%016x' % (key[1],key[0])
	return ret

def loadModules( libs, dir ):	
	modules ={}
	for x in libs:
		dll = CDLL(path.join(dir,x))
		obj = {}
		obj = {'obj':dll,'item':dll.newFun()}
		modules[getKey(obj)] = obj
	return modules
	
def destroy( obj ):
	obj['obj'].destroy(obj['item'])
	obj['item']=c_void_p(0)

def run(obj,argv):
	obj['obj'].run(obj['item'],argv)
	
libs = ['libfun1.so','libfun2.so','libfun3.so']
pythons = [{'module':'usrt','class':'dummycapability'}]
modules = loadModules( libs, sys.argv[1] )
argv = (c_int*2)(0,0)
print modules
for key, m in modules.items():
	m['obj'].run(m['item'],byref(argv))
	destroy(m)

sys.path.append(path.abspath('.'))
import usrt.worker
import usrt.dummycapability



class task(Structure):
  _fields_ = [
    ("ID",c_longlong),
    ("ufrom",c_longlong),
    ("to",c_longlong),
    ("noE",c_longlong),
    ("noL",c_longlong),
    ("valid",c_longlong),
    ("argv",c_void_p),
    ("lock",c_int),
    ]
argvs = {'libs':libs,'dir':sys.argv[1],'pythons':pythons} 
worker1 = usrt.worker.worker( argvs)
for key, v in worker1.items():
	m = worker1.getModule(key)
	worker1.run(m,byref(argv))
print "-------------------------------"

modules2 = loadModules( libs, sys.argv[1] )
for key, m in modules2.items():
	run(m,byref(argv))
for key, m in modules2.items():
	run(m,byref(argv))
ap = POINTER(task)()
dll = CDLL(path.join(sys.argv[1],"libcontainerapi.so"))
k=0;
while	dll.readTask(byref(ap))==0:
  print k 
  print ap.contents.ID
  k+=1