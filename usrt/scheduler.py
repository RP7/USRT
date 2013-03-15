from ctypes import *  
from os import path
import time
from usrt.logger import log,md5key

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
		self.key = md5key(self.__module__)
		self._dll = CDLL(path.join("work","libcontainerapi.so"))

	def _pushTask(self,globeDict,task ):
		globeDict['tasks'].update({task['ID']:task})
		globeDict['early'].put((task['noE'],task['ID']))
	
	def _pushTaskNoWait(self,globeDict,task ):
		globeDict['tasks'].update({task['ID']:task})
		for k,v in globeDict['workers'].items():
			if task['key'] in v['capabilities']:
				self._pushToWorker(v,task)
		
	def _pushToWorker(self,worker,task):
		worker['queue'].put((task['noL'],task['ID']))
		
	def _mygetattr(self,struct,field):
		k=getattr(struct, field)
		if field=="key":
			return '%016x%016x' % (k[1],k[0])
		else:
			return k
		
	def _getdict(self,struct):
		return dict((field, self._mygetattr(struct, field)) for field, _ in struct._fields_)

	def _buildVertex(self,vertexes,task):
		ufrom = task['ufrom'] 
		if ufrom in vertexes:
			vertexes[ufrom]['tasks'].append(task)
		else:
			vertexes[ufrom]={'dep':0,'tasks':[task]}
		to = task['to']
		if to in vertexes:
			vertexes[to]['dep']+=1
		else:
			vertexes[to]={'dep':1,'tasks':[]}
		
	def run(self,argv):
		ap = POINTER(Task)()
		k=0;
		vertexes = argv['vertexes']
		while	self._dll.readTask(byref(ap))==0:
			task=self._getdict(ap.contents)
			self._buildVertex(vertexes,task)
			
		for k,v in vertexes.items():
			if v['dep']==0:
				for l in range(0,len(v['tasks'])):
					self._pushTask(argv,v['tasks'][l])
				del vertexes[k]
		now = round(time.time()*1000000.)
		q = argv['early']
		q.put((now,'1'))
		_,id = q.get()
		while id!='1':
			task=argv['tasks'][id]
			log("info", "task "+task['key']+" is ready")
			for k,v in argv['workers'].items():
				if task['key'] in v['capabilities']:
					self._pushToWorker(v,task)
			_,id = q.get()
		task={'ID':'1' ,'key':'a449428a47383acfd816603b55c3cfa7','argv':argv,'noL':now+1000 }
		self._pushTaskNoWait(argv,task) 

			
	def getKey(self):
		return self.key
		
	def destroy(self):
		return
		
def newFun():
	return Scheduler()
	