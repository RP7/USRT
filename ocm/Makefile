#CC=arm-xilinx-linux-gnueabi-gcc 
#CPP=arm-xilinx-linux-gnueabi-g++ 

all: ocm_io.o test.o fpga_io.o
	$(CC) -o test ocm_io.o fpga_io.o test.o  -lm -lpthread

test.o : test.c 
	$(CC) -c test.c

fpga_io.o : fpga_io.c fpga_io.h
	$(CC) -c fpga_io.c
	
ocm_io.o : ocm_io.c ocm_io.h
	$(CC) -c ocm_io.c

rx_udp : fpga_io.c fpga_io.h ocm_io.c ocm_io.h rx_udp.cpp
	g++ -o rx_udp fpga_io.c fpga_io.h ocm_io.c ocm_io.h rx_udp.cpp		

tx_udp : fpga_io.c fpga_io.h ocm_io.c ocm_io.h tx_udp.cpp
	g++ -o tx_udp fpga_io.c fpga_io.h ocm_io.c ocm_io.h tx_udp.cpp		

clean:
	rm -f *.o
	rm -f *.so
	rm -f tx_udp
	rm -f rx_udp
	
lib: fpga_io.c fpga_io.h ocm_io.c ocm_io.h
	gcc -o q7ocm_map.so -shared -fPIC fpga_io.c ocm_io.c

sat: rx_udp tx_udp
	
