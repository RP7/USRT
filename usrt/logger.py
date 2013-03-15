import time
import json
import Queue
import hashlib

logQ=Queue.Queue(0)

def log(level,object):
	t = time.time()
	logQ.put((t,level,json.dumps(object)))

def md5key(string):
	m = hashlib.md5() 
	m.update(string)
	return m.hexdigest()
	