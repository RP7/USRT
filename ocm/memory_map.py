import mmap
from ctypes import *
import threading


DEVNAME = '/dev/mem'

DEV_MEM = 0
FPGA_ADDR_BASE = 0x40000000
MEM_LEN = 65536
WH_OFFSET = 0xc020
RH_OFFSET = 0xc024
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


mem_condition = threading.Condition()
def dev_mem_init(rf_ring, resample_order):
    global DEV_MEM, DEV_OCM

    fd = open(DEVNAME,'r+b',buffering=0)
    DEV_MEM = mmap.mmap(fd.fileno(),MEM_LEN, offset=FPGA_ADDR_BASE)
    DEV_OCM = mmap.mmap(fd.fileno(),MEM_LEN, offset=DATA_ADDR_BASE)

    #dds_tx = 2**12
    #dds_rx = 2**12
    dds_tx = 0
    dds_rx = 0
    set_dds(dds_tx,dds_rx)

    set_rf_ring(rf_ring)
    set_sample_freq(resample_order)
    
    
    
dds = (c_uint*1)()
def set_dds(tx_dds, rx_dds):
    global DEV_MEM
    dds[0] = tx_dds
    DEV_MEM.seek(DDS_ADDR_Tx)
    DEV_MEM.write(dds)
    dds[0] = rx_dds
    DEV_MEM.seek(DDS_ADDR_Rx)
    DEV_MEM.write(dds)

rf_ring = (c_uint*1)()
def set_rf_ring(rf_ring_flag):
    DEV_MEM.seek(RF_RING_REG)
    rf_ring[0] = rf_ring_flag
    DEV_MEM.write(rf_ring)
    if rf_ring_flag == 0:
        print 'FGAP RING_BACK!!'
    else:
        print 'Using RF !'

sample_rate_opt = (c_uint*1)()
def set_sample_freq(order):
    DEV_MEM.seek(SAMPLE_RATE_OPT)
    sample_rate_opt[0] = order
    DEV_MEM.write(sample_rate_opt)

    basic_rate = 120e3
    sample_rate = basic_rate * (2**order)
    
    print 'Sampling Frequency', sample_rate

w_offset = (c_uint*1)()
def get_write_count():
    global DEV_MEM, w_offset
    DEV_MEM.seek(WH_OFFSET)
    memmove(w_offset,DEV_MEM.read(4),4)
    return w_offset[0]

r_offset = (c_uint*1)()
def get_read_count():
    global DEV_MEM
    DEV_MEM.seek(RH_OFFSET)
    memmove(r_offset,DEV_MEM.read(4),4)
    return r_offset[0]

def read_data(offset, data_len):
    global DEV_OCM
    DEV_OCM.seek(READ_OFFSET + offset)
    data_buf = DEV_OCM.read(data_len)
    return data_buf

def write_data(offset, data_buf):
    global DEV_OCM
    DEV_OCM.seek(WRITE_OFFSET + offset)
    DEV_OCM.write(data_buf)
    return 0

def circle_test(tx_offset, rx_offset, data_len):
    DEV_OCM.move(READ_OFFSET + rx_offset, WRITE_OFFSET + tx_offset, data_len)
