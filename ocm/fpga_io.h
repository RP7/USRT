#ifndef _FPGA_IO_H
#define _FPGA_IO_H

#define _FPGA_IO_(ZZ)		(*((int *)(g_FPGA_pntr + ZZ)))

extern int drvFGPA_OpenDev( const char *dev );
extern int drvFPGA_CloseDev( const int fd );
extern void *drvFPGA_Mmap(int fd , int addr_base);
extern int drvFPGA_Munmap( void *pntr , int len );
extern int drvFPGA_Init( void );
extern int drvFPGA_Read( int io_addr );
extern int drvFPGA_ReadMem( int start_addr, int *rd_data, int num );
extern int drvFPGA_Write( int io_addr , int io_data );
extern int drvFPGA_WriteMem( int start_addr , int *wr_data, int num );
#endif
