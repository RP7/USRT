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
from usrt.logger import dump
import time

workers=[]
globeDict={}

def init( config ):
	global workers, globeDict
	fd=open(config,'r')
	globeDict=json.loads(fd.read())
	mutex = threading.Lock()
	globeDict.update({'mutex':mutex})
	earlyQ=Queue.PriorityQueue(0)
	globeDict.update({'early':earlyQ})
	globeDict.update({'vertexes':{}})
	overQ=Queue.Queue(0)
	globeDict.update({'overQ':overQ})
	
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
	task={'ID':'1' ,'key':usrt.logger.md5key('usrt.scheduler'),'argv':globeDict }
	testPushTask(globeDict,task) 
	task={'ID':'2' ,'key':usrt.logger.md5key('exampleFun1'),'argv':byref(argv) }
	testPushTask(globeDict,task)
	
	while True:
		dump([" run"])