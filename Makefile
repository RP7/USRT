INC=include
PINC=/usr/include/python2.7
FLAG=-fPIC -fpermissive -Lwork 
LDFLAG=-ldl -lpthread

work/libfun1.so:examples/fun1.cpp work/libcontainer.so examples/LTEDownLinkTransMock.c work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -Lwork -lcontainer -lmd5api -o work/libfun1.so examples/fun1.cpp examples/LTEDownLinkTransMock.c work/libmd5api.so work/libcontainer.so

#work/libPssMod.so:examples/capPssMod.cpp work/libmd5api.so
#	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api $< -o $@
#	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api -o work/libPssMod.so examples/capPssMod.cpp work/libmd5api.so

	
work/libfun2.so:examples/fun2.cpp work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libfun2.so examples/fun2.cpp work/libmd5api.so work/libcontainer.so
	
work/libfun3.so:examples/fun3.cpp work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libfun3.so examples/fun3.cpp work/libmd5api.so

CONTAINERSRC = 	usrt/mem/MapMem.cpp \
								utils/CPBuffer.cpp \
								usrt/task/USRTTask.cpp \
								usrt/worker/USRTWorkers.cpp \
								usrt/worker/USRTTaskQueue.cpp \
								usrt/worker/USRTCapabilityBearer.cpp \
								usrt/mem/USRTMem.cpp \
								usrt/mem/USRTFifo.cpp \
								usrt/container/ukey.c \
								usrt/container/globe.c

USRTSRC = 	usrt/mem/MapMem.cpp \
		  			utils/CPBuffer.cpp \
						usrt/task/USRTTask.cpp \
						usrt/workers/USRTWorkers.cpp \
						usrt/workers/USRTTaskQueue.cpp \
						usrt/workers/USRTCapabilityBearer.cpp \
						usrt/mem/USRTMem.cpp \
						usrt/mem/USRTFifo.cpp \
						usrt/log/Log.cpp \
						usrt/container/ukey.c \
						usrt/container/globe.c


work/libUSRT.so: ${USRTSRC} work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libUSRT.so ${USRTSRC} work/libmd5api.so $(LDFLAG)

								
work/libcontainer.so: ${CONTAINERSRC} work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libcontainer.so ${CONTAINERSRC} work/libmd5api.so

LTEDownExample:work/ltetest 
work/ltetest: examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c usrt/container/task.c work/libcontainer.so work/libmd5api.so
	g++ -I${INC} -Lwork -o work/ltetest examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c usrt/container/task.c  work/libcontainer.so work/libmd5api.so

work/libcontainerapi.so :	usrt/containerAPI.c work/libcontainer.so
	g++ -shared  ${FLAG} -I${INC} -Lwork -lcontainer -o work/libcontainerapi.so usrt/containerAPI.c work/libcontainer.so
	
MD5LIBSRC = utils/md5.c utils/md5api.c

work/libmd5api.so: ${MD5LIBSRC}
	g++ -I${INC} ${FLAG} -shared -o work/libmd5api.so ${MD5LIBSRC}

ExamplesCapabilities := $(wildcard examples/cap*.cpp)
ExamplesLibs := $(patsubst %.cpp,%.so,$(subst examples/cap,work/lib,$(ExamplesCapabilities)))
$(ExamplesLibs): %.so: $(patsubst %.so,%.cpp,$(subst work/lib,examples/cap,$@)) work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -Lwork $(patsubst %.so,%.cpp,$(subst work/lib,examples/cap,$@)) -o $@  -lmd5api
example:$(ExamplesLibs)

example1:work/libfun1.so work/libfun2.so work/libfun3.so work/libcontainerapi.so work/libmd5api.so $(LTELibs)



WorkersInternalCapabilities := $(wildcard usrt/workers/cap*.cpp)
WorkersInternalLibs := $(patsubst %.cpp,%.so,$(subst usrt/workers/cap,work/lib,$(WorkersInternalCapabilities)))

$(WorkersInternalLibs): %.so: $(patsubst %.so,%.cpp,$(subst work/lib,usrt/workers/cap,$@)) work/libmd5api.so work/libUSRT.so
	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api $(patsubst %.so,%.cpp,$(subst work/lib,usrt/workers/cap,$@)) work/libmd5api.so work/libUSRT.so -o $@


DUMPMEMSRC= \
  usrt/mem/MapMem.cpp \
  utils/CPBuffer.cpp \
  usrt/task/USRTTask.cpp \
  usrt/mem/USRTMem.cpp \
  usrt/mem/USRTFifo.cpp \
  usrt/container/ukey.c \
  usrt/container/globe.c
  
work/dumpMem:work/libUSRT.so work/libmd5api.so utils/dumpMem.cpp
	g++ -I${INC} ${FLAG} -Lwork  -lmd5api -o work/dumpMem utils/dumpMem.cpp work/libUSRT.so work/libmd5api.so $(LDFLAG)

work/heapcheck:usrt/workers/USRTTaskQueue.cpp work/libcontainer.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -D__HEAPTEST -o work/heapcheck  usrt/workers/USRTTaskQueue.cpp work/libmd5api.so work/libcontainer.so 

work/dumpQueue: utils/dumpQueue.cpp usrt/workers/USRTTaskQueue.cpp work/libcontainer.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/dumpQueue utils/dumpQueue.cpp usrt/workers/USRTTaskQueue.cpp work/libmd5api.so work/libcontainer.so 

work/dumpKey: utils/dumpCapKey.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC}  ${FLAG} -o work/dumpKey utils/dumpCapKey.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/findCapByKey: utils/findCapByKey.cpp usrt/workers/USRTCapabilityBearer.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC}  ${FLAG} -o work/findCapByKey utils/findCapByKey.cpp usrt/workers/USRTCapabilityBearer.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/keeperCheck: utils/keeperCheck.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/keeperCheck utils/keeperCheck.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/workers: usrt/workers/workers.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/workers usrt/workers/workers.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/configWorkers: utils/configWorkers.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/configWorkers utils/configWorkers.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/pushTask: utils/pushTask.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/pushTask utils/pushTask.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/initLog: utils/initLog.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/initLog utils/initLog.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/dumpLog: utils/dumpLog.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/dumpLog utils/dumpLog.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)

work/delayLog: utils/delayLog.cpp work/libUSRT.so work/libmd5api.so
	g++ -I${INC} ${FLAG} -o work/delayLog utils/delayLog.cpp work/libmd5api.so work/libUSRT.so $(LDFLAG)


UTILS = work/configWorkers \
  work/workers \
  work/findCapByKey \
  work/dumpKey \
  work/keeperCheck \
  work/pushTask \
  work/dumpMem \
  work/initLog \
  work/dumpLog \
  work/delayLog
  

worker:$(WorkersInternalLibs) $(UTILS)

.PHONY : clean
clean:
	rm work/* -f
	python scripts/prepare.py work

		
