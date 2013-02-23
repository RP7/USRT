import hashlib
import time
import json
import sys
from os import path 

def md5key( mstr='' ):
	now = "%16.6f" % time.time()
	m = hashlib.md5() 
	m.update(now)
	if mstr:
		m.update(mstr)
	return m.hexdigest()

mainkey = md5key('main')
path = path.join(sys.argv[1],mainkey)
fd = open(path,'w')

worker1 = { 'capabilities':['libfun1.so','libfun2.so'], 'tag':md5key('1') }
worker2 = { 'capabilities':['libfun1.so','libfun3.so'], 'tag':md5key('2') }
worker3 = { 'capabilities':['libfun2.so','libfun3.so'], 'tag':md5key('3') }

main = { 'tag':mainkey, 'workers':[ worker1, worker2, worker3 ] }

fd.write( json.dumps( main ) )
fd.close()



  
