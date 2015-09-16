#include <stdio.h>

#include "Q7Mem.h"
#include "Q7MemAPI.h"

#include "easySocket.h"

int main(int argc, char*argv[] )
{
  Q7Mem *tx_mem = attachQ7Mem( "tx_udp.d" );
  dumpQ7Mem( tx_mem );
  Q7Mem *rx_mem = attachQ7Mem( "rx_udp.d" );
  dumpQ7Mem( rx_mem );
  int port;
  unsigned int sendBuf[304];
  unsigned int recvBuf[304];
  sscanf( argv[2],"%d",&port );
  printf("port %d,aizeof int %d\n",port, sizeof(int));
  udpClient udp( argv[1], port );
  sendBuf[0] = sendBuf[1] = 0x7f7f7f7f;
  udp.send( (char *)sendBuf, 8 );
  int len = 0;
  unsigned int last_send;
  while( (len=udp.recv( (char*)recvBuf, 1216 )) == 1216 )
 {
    if( recvBuf[0]==0x7f7f7f7f && recvBuf[1]==1200 )
    {
      unsigned int idx = recvBuf[2];
      long long from = idx * 300 * 4;
      int *p = (int *)rx_mem->getBuf( from, 1200 );
      memcpy( p, &recvBuf[4], 1200 );
      unsigned int free = recvBuf[3] - 2;
      if((free-last_send)<60)
      {
        for( unsigned int x=last_send;x<free;x++ )
        {
          from = x * 300 * 4;
          p = (int *)tx_mem->getBuf( from, 1200 );
          sendBuf[0] = 0x7f7f7f7f;
          sendBuf[1] = 1200;
          sendBuf[2] = x;
          sendBuf[3] = 0;
          memcpy( &sendBuf[4], p, 1200 );
          len = udp.send( (char *)sendBuf, 1216 );
          if( len!=1216 )
          {
            printf("udp send failure,%d\n",len);
          }
        }
      }
      else
      {
        printf(" OCM send buf overflow,%d,%d,%d\n",free-last_send, free, last_send);
      }
      last_send = free;

    }
    else
      printf("error in packet head, dropped %x %d %x %x\n",recvBuf[0],recvBuf[1],recvBuf[2],recvBuf[3]);
 }
 
}
