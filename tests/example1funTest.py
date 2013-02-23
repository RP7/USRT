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
argv = (c_longlong*2)(0x3031323334353637,0x3031323334353637)
print modules
for key, m in modules.items():
	m['obj'].run(m['item'],byref(argv))
	destroy(m)

sys.path.append(path.abspath('.'))
import usrt.worker
import usrt.dummycapability

modules2 = loadModules( libs, sys.argv[1] )
for key, m in modules2.items():
	run(m,byref(argv))

argvs = {'libs':libs,'dir':sys.argv[1],'pythons':pythons} 
worker1 = usrt.worker.worker( argvs)
for key, v in worker1.items():
	m = worker1.getModule(key)
	worker1.run(m,byref(argv))	