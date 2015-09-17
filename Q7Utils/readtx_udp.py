import struct

f = open('/tmp/tx_udp.d')
buf = f.read(4096)
buf = f.read(16)

for i in range(8):
  h, = struct.unpack_from( "H",buf, i*2 )
  print hex(h)



