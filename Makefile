INC=include

example1:work/libfun1.so work/libfun2.so work/libfun3.so

work/libfun1.so:examples/fun1.cpp
	g++ -I${INC} -shared -o work/libfun1.so examples/fun1.cpp
	
work/libfun2.so:examples/fun2.cpp
	g++ -I${INC} -shared -o work/libfun2.so examples/fun2.cpp
	
work/libfun3.so:examples/fun3.cpp
	g++ -I${INC} -shared -o work/libfun3.so examples/fun3.cpp
	