import time
import json
import Queue
import hashlib
import threading

logQ=Queue.Queue(0)

def log(level,object):
	t = time.time()
	who = threading.currentThread().name
	logQ.put((t,who,level,json.dumps(object)))

def md5key(string):
	m = hashlib.md5() 
	m.update(string)
	return m.hexdigest()

def dump(items):
	t,who,level,str=logQ.get()
	if level in items:
		print "[%s]%s:%s:%06d:%s" %(level,who,time.ctime(int(t)),int((t-int(t))*1000000.),str)
	