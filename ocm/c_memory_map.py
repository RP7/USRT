import mmap
from ctypes import *
import threading
mmap_lib = CDLL('./q7ocm_map.so')

DEVNAME = '/dev/mem'

DEV_MEM = 0
FPGA_ADDR_BASE = 0x40000000
MEM_LEN = 65536
SAMPLE_CNT_OFFSET = 0xc020
BLOCK_CNT_OFFSET = 0xc024
DDS_ADDR_Tx = 0xc000
DDS_ADDR_Rx = 0xc004
RF_RING_REG = 0xc008
SAMPLE_RATE_OPT = 0xc014

DEV_OCM = 0
DATA_ADDR_BASE = 0xfffc0000
READ_OFFSET = 0x0000
WRITE_OFFSET = 0x2000
W_R_STEP = 4096

##DEVNAME = '/dev/mem'
##FPGA_ADDR_BASE = 0x40000000
##MEM_LEN = 65536
##READ_OFFSET = 0xe000
##WRITE_OFFSET = 0xd000
##WH_OFFSET = 0xc020
##RH_OFFSET = 0xc024
##DDS_ADDR_Tx = 0xc000
##DDS_ADDR_Rx = 0xc004
##RF_RING_REG = 0xc008
##SAMPLE_RATE_OPT = 0xc014

##DEVNAME = 'hello.txt'
##FPGA_ADDR_BASE = 0
##MEM_LEN = 8192
##READ_OFFSET = 0x100
##WRITE_OFFSET = 0x1100
##WH_OFFSET = 0x0
##RH_OFFSET = 0x4
##DDS_ADDR_Tx = 0x8
##DDS_ADDR_Rx = 0xc
##RF_RING_REG = 0x10

#BLK_SIZE = 696 # number of c_short of real and imag each BLK
BLK_SIZE = 600 # number of c_short of real and imag each BLK
TX_BUF_SIZE = BLK_SIZE*2/2 # two blk of int32
RX_BUF_SIZE = BLK_SIZE*2/2


def dev_mem_init(rf_ring, resample_order):
    mmap_lib.drvOCM_Init()
    mmap_lib.drvFPGA_Init()

    set_rf_ring(rf_ring)
    set_sample_freq(resample_order)


rf_ring = (c_uint*1)()
def set_rf_ring(rf_ring_flag):
    mmap_lib.drvFPGA_Write(RF_RING_REG,rf_ring_flag)
    if rf_ring_flag == 0:
        print 'FGAP RING_BACK!!'
    else:
        print 'Using RF !'

def set_sample_freq(order):
    mmap_lib.drvFPGA_Write(SAMPLE_RATE_OPT,order)

    basic_rate = 120e3
    sample_rate = basic_rate * (2**order)
    
    print 'Sampling Frequency', sample_rate

def get_write_count():
    return mmap_lib.drvFPGA_Read(WH_OFFSET)

def read_data(offset, data_len):
    data_buf = create_string_buffer(data_len)
    mmap_lib.drvOCM_ReadMem(offset, data_buf, data_len/8)
    return string_at(data_buf,data_len)

data_buf = (c_uint*300)()
def write_data(offset, data_string):
    #memmove(data_buf,data_string,1200)
    mmap_lib.drvOCM_WriteMem(offset, data_buf, 1200/8)


