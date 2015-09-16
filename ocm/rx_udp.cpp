#include "easySocket.h"
#include "fpga_io.h"
#include "ocm_io.h"
#include "satConfig.h"

//const static char msg_len[5]="1200";

class rx:public udpServer
{
	private:
		int *buf;
	public:
		rx( unsigned short port ):udpServer( port ) 
		{
			buf = new int[300+4];
			memset( buf, 0, sizeof(IpInfo_t) );
			drvOCM_WriteMem( PEERIPINFO_OFFSET, buf, (sizeof(IpInfo_t)+3)/4 );
		};
		~rx()
		{
			delete buf;		
		};

		void write( int point)
		{
				
				int len = recv( (char *)buf, 1216 );
				if( buf[0]==0x7f7f7f7f )
				{
					if( len==1216 && buf[1]== 1200 )
					{	
						int Num = buf[2];
						Num = Num&0x3f;
						int error = (point - Num)&0x3f;
						if( error == 0 || error == 63)
							printf("buffer over flow \n");
						drvOCM_WriteMem( 0x20000 + Num*2048, &buf[4], 300 );
					}
					else if( len==8 && buf[1]==0x7f7f7f7f )
					{
						printf(" maybe connect \n");
						int point = drvFPGA_Read( BLOCK_CNT_OFFSET );
						int free = (point-2)&0x3f;	
						peer.wlast = free;
						drvOCM_WriteMem( PEERIPINFO_OFFSET, (int*)&(udpServer::peer), (sizeof(IpInfo_t)+3)/4 );	
					}
					
					drvOCM_WriteMem( PEERIPINFO_OFFSET, (int*)&(udpServer::peer), (sizeof(IpInfo_t)+3)/4 );				
			
				}
		};	
};

int main()
{
	
	drvFPGA_Init();
	drvOCM_Init();
	rx aRx( SATPORT );
	int point = drvFPGA_Read( BLOCK_CNT_OFFSET );
	int last_point = point;
	int wlast = (point-2)&0x3f;
	int cnt = 0;
	int flag = 0;
	drvOCM_Write( STOP_FLAG , flag );
	while( !flag )
	{
		point = drvFPGA_Read( BLOCK_CNT_OFFSET );
		int free = (point-2)&0x3f;
		int buflen = (free-wlast)&0x3f;
		aRx.write( point);
		flag = drvOCM_Read( STOP_FLAG );
	}
}


