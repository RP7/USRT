from ctypes import *  
from os import path

class Task(Structure):
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

class Scheduler:
	def __init__(self):
		self.key = "a449428a47383acfd816603b55c3cfa7"
		self._dll = CDLL(path.join("work","libcontainerapi.so"))

	def _pushTask(self,globeDict,task ):
		globeDict['tasks'].update({task['ID']:task})
		for k,v in globeDict['workers'].items():
			if task['key'] in v['capabilities']:
				v['queue'].put((k,task['ID']))

	def _mygetattr(self,struct,field):
		k=getattr(struct, field)
		if field=="key":
			return '%016x%016x' % (k[1],k[0])
		else:
			return k
		
	def _getdict(self,struct):
		return dict((field, self._mygetattr(struct, field)) for field, _ in struct._fields_)


	def run(self,argv):
		ap = POINTER(Task)()
		k=0;
		while	self._dll.readTask(byref(ap))==0:
			print k 
			print self._getdict(ap.contents)
			k+=1
		task={'ID':'1' ,'key':'a449428a47383acfd816603b55c3cfa7','argv':argv }
		self._pushTask(argv,task) 

			
	def getKey(self):
		return self.key
		
	def destroy(self):
		return
		
def newFun():
	return Scheduler()
	