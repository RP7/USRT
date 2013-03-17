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

workingDir='work'
dummy={'module':'usrt','class':'dummycapability'}
LTEcap = ['libDCHMap.so', 'libPssMap.so', 'libDCHCoding.so', 'libDCHMod.so', 'libPssMod.so']
LibforW1 = ['libfun1.so','libfun2.so']
LibforW1.extend(LTEcap)
LibforW2 = ['libfun1.so','libfun3.so']
LibforW2.extend(LTEcap)
scheduler={'module':'usrt','class':'scheduler'}
worker1 = { 'dir':workingDir,'libs':LibforW1, 'tag':md5key('1'),'pythons':[dummy,scheduler] }
worker2 = { 'dir':workingDir,'libs':LibforW2, 'tag':md5key('2'),'pythons':[] }
worker3 = { 'dir':workingDir,'libs':LTEcap, 'tag':md5key('3'),'pythons':[] }

main = { 'tag':mainkey, 'workers':{ worker1['tag']:worker1, worker2['tag']:worker2, worker3['tag']:worker3 }, 'tasks':{} }
for k,v in main['workers'].items():
  v.update({'capabilities':[],'wait':[]})
fd.write( json.dumps( main, indent=2, separators=(',', ': ')))
fd.write('\n')
fd.close()



  
