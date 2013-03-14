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
"""
	int64 ID;
	int64 key[2];
	int64 from;
	int64 to;
	utime_t noE;
	utime_t noL;
	utime_t valid;
	void *argv;
	int lock;
"""
class task(Structure):
  _fields_ = [
    ("ID",c_longlong),
    ("key",c_longlong*2),
    ("ufrom",c_longlong),
    ("to",c_longlong),
    ("noE",c_longlong),
    ("noL",c_longlong),
    ("valid",c_longlong),
    ("argv",c_void_p),
    ("lock",c_int),
    ]
argvs = {'libs':libs,'dir':sys.argv[1],'pythons':pythons,"capabilities":[],"queue":"","tag":"worker1"} 
worker1 = usrt.worker.worker( {"workers":{"worker1":argvs},"tasks":{},"mutex":""}, "worker1" )
for key, v in worker1.items():
	m = worker1.getModule(key)
	worker1._run(m,byref(argv))
print "-------------------------------"

def mygetattr(struct, field):
	k=getattr(struct, field)
	if field=="key":
		return '%016x%016x' % (k[1],k[0])
	else:
		return k
		
def getdict(struct):
    return dict((field, mygetattr(struct, field)) for field, _ in struct._fields_)


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
  print getdict(ap.contents)
  k+=1