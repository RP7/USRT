from ctypes import *
import c_memory_map
mmap_lib = CDLL('./q7ocm_map.so')

mmap_lib.drvOCM_Init()
mmap_lib.drvFPGA_Init()
mmap_lib.drvFPGA_Write(c_memory_map.SAMPLE_RATE_OPT,4)

i = 0
last_count = 0
for j in range(100):
    for i in range(20000):
        write_count = mmap_lib.drvFPGA_Read(c_memory_map.BLOCK_CNT_OFFSET)
    print 'read time', (write_count - last_count)&0xffff, write_count 
    last_count = write_count


##
##rf_ring = 1 # '1': RF ring; '0': FPGA ring
##resample_order = 4 # '0/1/2/3/4: 120k/240k/480k/960k/1.92m
##memory_map.dev_mem_init(rf_ring, resample_order)
##
##r_offset = (c_uint*1)
##def get_read_c():
##    memory_map.DEV_MEM.seek(memory_map.RH_OFFSET)
##    a = memory_map.DEV_MEM.read(4)
##    #a = create_string_buffer(4)
##    return r_offset.from_buffer( a, 0 )
##
##i = 0
##last_count = 0
##while True:
##    for i in range(1000):
##        write_count = get_read_c()
##    print 'read time', write_count[0] - last_count
##    last_count = write_count[0]

