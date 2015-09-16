from ctypes import *
import c_memory_map
mmap_lib = CDLL('./q7ocm_map.so')

mmap_lib.drvOCM_Init()
mmap_lib.drvFPGA_Init()
mmap_lib.drvFPGA_Write(c_memory_map.SAMPLE_RATE_OPT,4)

mem_w_data = (c_int*(64*300))()
mem_r_data = (c_int*(64*300))()
for i in range(len(mem_w_data)):
	mem_w_data[i]=i
wlast = 0
rlast = 0
memspeed = 0
log = [0]*(64*300)
def write( buf, start, end ):
    for i in range( start, end ):
        mmap_lib.drvOCM_WriteMem( 0x20000 + i*2048, addressof(buf)+i*1200, 300 )
def read( buf, start, end ):
    for i in range( start, end ):
        mmap_lib.drvOCM_ReadMem( i*2048, addressof(buf)+i*1200, 300 )
        
for i in range(1000):
    point = mmap_lib.drvFPGA_Read( c_memory_map.BLOCK_CNT_OFFSET )
    free = (point-2)&0x3f
    if wlast>free:
        write( mem_w_data, wlast, 0x40 )
        read( mem_r_data, rlast, 0x40 )
        write( mem_w_data, 0, free )
        read( mem_r_data, 0, free )
    else:
        write( mem_w_data, wlast, free )
        read( mem_r_data, rlast, free )
    memspeed += (free - wlast)&0x3f
    wlast = free
    rlast = free
for i in range(64*300):
    x = (mem_r_data[i]-mem_w_data[i])%(64*300)
    if x==18000:
        print i,
    log[x] += 1
slog = []
slog[:] = log[:]
    
slog.sort(reverse=True)
    
print 'read time', memspeed, log.index(slog[0]), slog[:10], log.index(slog[1])
    
    
