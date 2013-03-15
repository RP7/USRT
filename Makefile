INC=include
PINC=/usr/include/python2.7
FLAG=-fPIC -fpermissive
example1:work/libfun1.so work/libfun2.so work/libfun3.so work/libcontainerapi.so

work/libfun1.so:examples/fun1.cpp work/libcontainer.so examples/LTEDownLinkTransMock.c
	g++ -I${INC} ${FLAG} -shared -Lwork -lcontainer -o work/libfun1.so examples/fun1.cpp examples/LTEDownLinkTransMock.c work/libcontainer.so
	
work/libfun2.so:examples/fun2.cpp
	g++ -I${INC} ${FLAG} -shared -o work/libfun2.so examples/fun2.cpp
	
work/libfun3.so:examples/fun3.cpp
	g++ -I${INC} ${FLAG} -shared -o work/libfun3.so examples/fun3.cpp

clean:
	rm work/*

CONTAINERSRC = usrt/container/task.c \
								usrt/container/ukey.c \
								usrt/container/usrtmem.c \
								usrt/container/globe.c
work/libcontainer.so: ${CONTAINERSRC}
	g++ -I${INC} ${FLAG} -shared -o work/libcontainer.so ${CONTAINERSRC}

LTEDownExample:work/ltetest 
work/ltetest: examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c work/libcontainer.so
	g++ -I${INC} -Lwork -lcontainer -o work/ltetest examples/LTEDownLinkTrans.cpp examples/LTEDownLinkTransMock.c work/libcontainer.so

work/libcontainerapi.so :	usrt/containerAPI.c work/libcontainer.so
	g++ -shared  ${FLAG} -I${INC} -Lwork -lcontainer -o work/libcontainerapi.so usrt/containerAPI.c work/libcontainer.so
	
MD5LIBSRC = utils/md5.c utils/md5api.c

work/libmd5api.so: ${MD5LIBSRC}
	g++ -I${INC} ${FLAG} -shared -o work/libmd5api.so ${MD5LIBSRC}
	