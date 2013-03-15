from ctypes import *  
from os import path
import threading
from usrt.logger import log

class Worker(threading.Thread):

	def __init__( self,argvs,whoamI ):
		argv = argvs['workers'][whoamI]
		self._modules ={}
		self._loadModules( argv['libs'],argv['dir'] )
		self._loadPythons( argv['pythons'] )
		for k,v in self._modules.items():
			argvs['workers'][whoamI]['capabilities'].append(k)
		self._jobq = argvs['workers'][whoamI]['queue']
		self._globeTask = argvs['tasks']
		self._mutex = argvs['mutex']
		threading.Thread.__init__(self,name=argv['tag'])
		
	def getKey( self,obj ):
		key = (c_ulonglong*2)()
		if( obj.has_key('item') ):
			obj['obj'].getKey(obj['item'],byref(key))
			ret = '%016x%016x' % (key[1],key[0])
		else:
			ret = obj['obj'].getKey()
		return ret

	def _loadModules( self,libs,dir ):	
		for x in libs:
			dll = CDLL(path.join(dir,x))
			obj = {}
			obj = {'obj':dll,'item':dll.newFun()}
			self._modules[self.getKey(obj)] = obj
			log("info","load :"+x+"("+self.getKey(obj)+")")
			
	def _loadPythons( self,libs ):	
		for x in libs:
			m = __import__(x['module'])
			obj = {}
			obj = {'obj':m.__dict__[x['class']].newFun()}
			self._modules[self.getKey(obj)] = obj
			log("info","load :"+x['class']+"("+self.getKey(obj)+")")
			
	def destroy( self,obj ):
		if( obj.has_key('item') ):
			obj['obj'].destroy(obj['item'])
			obj['item']=c_void_p(0)
		else:
			obj['obj'].destroy()

	def _run( self,obj,argv ):
		if( obj.has_key('item') ):
			obj['obj'].run(obj['item'],argv)
		else:
			obj['obj'].run(argv)
	
	def getModule( self,key ):
		return self._modules[key];
		
	def items(self):
		return self._modules.items()

	def run(self):
		while True:
			(k,tag)=self._jobq.get()
			item={}
			if self._mutex.acquire():
				if tag in self._globeTask and self._globeTask[tag]['key'] in self._modules:
					item.update( self._globeTask[tag] )
					del self._globeTask[tag]
				self._mutex.release()
			if item!={}:
				self._run(self._modules[item['key']],item['argv'])
			
def worker( argv, k ):
	return Worker( argv, k )