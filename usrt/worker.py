from ctypes import *  
from os import path

class Worker:

	def __init__( self,argv ):
		self.modules ={}
		self.loadModules( argv['libs'],argv['dir'] )
		self.loadPythons( argv['pythons'] )
		 
	def getKey( self,obj ):
		key = (c_longlong*2)()
		if( obj.has_key('item') ):
			obj['obj'].getKey(obj['item'],byref(key))
			ret = '%016x%016x' % (key[1],key[0])
		else:
			ret = obj['obj'].getKey()
		return ret

	def loadModules( self,libs,dir ):	
		for x in libs:
			dll = CDLL(path.join(dir,x))
			obj = {}
			obj = {'obj':dll,'item':dll.newFun()}
			self.modules[self.getKey(obj)] = obj
			
	def loadPythons( self,libs ):	
		for x in libs:
			m = __import__(x['module'])
			obj = {}
			obj = {'obj':m.__dict__[x['class']].newFun()}
			self.modules[self.getKey(obj)] = obj
			
	def destroy( self,obj ):
		if( obj.has_key('item') ):
			obj['obj'].destroy(obj['item'])
			obj['item']=c_void_p(0)
		else:
			obj['obj'].destroy()

	def run( self,obj,argv ):
		if( obj.has_key('item') ):
			obj['obj'].run(obj['item'],argv)
		else:
			obj['obj'].run(argv)
	
	def getModule( self,key ):
		return self.modules[key];
		
	def items(self):
		return self.modules.items()
	
def worker( argv ):
	return Worker( argv )