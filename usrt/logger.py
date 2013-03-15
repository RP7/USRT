import time
import json
import Queue

logQ=Queue.Queue(0)

def log(level,object):
	t = time.time()
	logQ.put((t,level,json.dumps(object)))
	