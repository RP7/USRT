INC=include
PINC=/usr/include/python2.7
FLAG=-fPIC -fpermissive

work/libfun1.so:examples/fun1.cpp work/libcontainer.so examples/LTEDownLinkTransMock.c work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -Lwork -lcontainer -lmd5api -o work/libfun1.so examples/fun1.cpp examples/LTEDownLinkTransMock.c work/libmd5api.so work/libcontainer.so

#work/libPssMod.so:examples/capPssMod.cpp work/libmd5api.so
#	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api $< -o $@
#	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api -o work/libPssMod.so examples/capPssMod.cpp work/libmd5api.so

	
work/libfun2.so:examples/fun2.cpp work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libfun2.so examples/fun2.cpp work/libmd5api.so work/libcontainer.so
	
work/libfun3.so:examples/fun3.cpp work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -o work/libfun3.so examples/fun3.cpp work/libmd5api.so

CONTAINERSRC = usrt/container/task.c \
								usrt/container/ukey.c \
								usrt/USRTMapMem.cpp \
								utils/CPBuffer.cpp \
								usrt/USRTmem.cpp \
								usrt/container/globe.c
work/libcontainer.so: ${CONTAINERSRC}
	g++ -I${INC} ${FLAG} -shared -o work/libcontainer.so ${CONTAINERSRC}

LTEDownExample:work/ltetest 
work/ltetest: examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c work/libcontainer.so work/libmd5api.so
	g++ -I${INC} -Lwork -o work/ltetest examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c work/libcontainer.so work/libmd5api.so

work/libcontainerapi.so :	usrt/containerAPI.c work/libcontainer.so
	g++ -shared  ${FLAG} -I${INC} -Lwork -lcontainer -o work/libcontainerapi.so usrt/containerAPI.c work/libcontainer.so
	
MD5LIBSRC = utils/md5.c utils/md5api.c

work/libmd5api.so: ${MD5LIBSRC}
	g++ -I${INC} ${FLAG} -shared -o work/libmd5api.so ${MD5LIBSRC}

LTECapabilities := $(wildcard examples/cap*.cpp)
LTELibs := $(patsubst %.cpp,%.so,$(subst examples/cap,work/lib,$(LTECapabilities)))

example1:work/libfun1.so work/libfun2.so work/libfun3.so work/libcontainerapi.so work/libmd5api.so $(LTELibs)


$(LTELibs): %.so: $(patsubst %.so,%.cpp,$(subst work/lib,examples/cap,$@)) work/libmd5api.so
	g++ -I${INC} ${FLAG} -shared -Lwork -lmd5api $(patsubst %.so,%.cpp,$(subst work/lib,examples/cap,$@)) work/libmd5api.so -o $@

DUMPMEMSRC=utils/dumpMem.cpp \
  usrt/USRTMapMem.cpp \
  utils/CPBuffer.cpp \
  usrt/USRTmem.cpp \
  usrt/container/task.c \
  usrt/container/ukey.c \
  usrt/container/globe.c
  
work/dumpMem:work/libcontainer.so $(DUMPMEMSRC)
	g++ -I${INC} -Lwork -o work/dumpMem work/libcontainer.so $(DUMPMEMSRC)
	
.PHONY : clean
clean:
	rm work/* -f
	python scripts/prepare.py work

		