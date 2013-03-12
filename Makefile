INC=include
PINC=/usr/include/python2.7

example1:work/libfun1.so work/libfun2.so work/libfun3.so

work/libfun1.so:examples/fun1.cpp
	g++ -I${INC} -shared -o work/libfun1.so examples/fun1.cpp
	
work/libfun2.so:examples/fun2.cpp
	g++ -I${INC} -shared -o work/libfun2.so examples/fun2.cpp
	
work/libfun3.so:examples/fun3.cpp
	g++ -I${INC} -shared -o work/libfun3.so examples/fun3.cpp

clean:
	rm work/*

CONTAINERSRC = usrt/container/task.c \
								usrt/container/ukey.c \
								usrt/container/usrttime.c \
								usrt/container/usrtmem.c \
								usrt/container/globe.c
work/libcontainer.so: ${CONTAINERSRC}
	g++ -I${INC} -shared -o work/libcontainer.so ${CONTAINERSRC}
	 		