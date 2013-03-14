from ctypes import *
import json
import sys
import Queue
import threading
from os import path
sys.path.append(path.abspath('.'))
import usrt.worker
import usrt.dummycapability
import usrt.scheduler

workers=[]
globeDict={}

def init( config ):
	global workers, globeDict
	fd=open(config,'r')
	globeDict=json.loads(fd.read())
	mutex = threading.Lock()
	globeDict.update({'mutex':mutex})

	for k,v in globeDict['workers'].items():
		q=Queue.PriorityQueue(0)
		v.update({'queue':q})
		workers.append( usrt.worker.worker( globeDict, k) )
	for w in workers:
		w.start()

def testPushTask( globeDict, task ):
	globeDict['tasks'].update({task['ID']:task})
	for k,v in globeDict['workers'].items():
		v['queue'].put((k,task['ID']))
	
if __name__ =="__main__":
	init(sys.argv[1])
	argv = (c_int*2)(0,0)
	task={'ID':'1' ,'key':'a449428a47383acfd816603b55c3cfa7','argv':globeDict }
	testPushTask(globeDict,task) 
	task={'ID':'2' ,'key':'7e95373c354213681318bb259d65ee19','argv':byref(argv) }
	testPushTask(globeDict,task) 
	
		