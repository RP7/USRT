#include "easySocket.h"
#include "fpga_io.h"
#include "ocm_io.h"
#include "satConfig.h"

const static char msg_len[5]="1200";

class tx:udpClient
{
	private:
		int *buf;
	public:
		tx( const char ip[], unsigned short port ):udpClient( ip, port )
		{
			buf = new int[300+4];
			memset( buf, 0, sizeof(IpInfo_t) );
			drvOCM_WriteMem( PEERIPINFO_OFFSET, buf, (sizeof(IpInfo_t)+3)/4 );
		};
		~tx()
		{
			delete buf;		
		};

		void read( int start, int end )
		{
			for( int i=start;i<end;i++ )
			{
				drvOCM_ReadMem( PEERIPINFO_OFFSET, (int*)&peer, (sizeof(IpInfo_t)+3)/4 );
				if( peer.len != 0 )
				{
					int point = drvFPGA_Read( BLOCK_CNT_OFFSET );
					int free = (point-2)&0x3f;
					int EmptyNum = (free - peer.wlast)&0x3f;
					if( EmptyNum>60 )
						EmptyNum = 0;
					drvOCM_ReadMem( (i&0x3f)*2048, &buf[4], 300 );
					buf[0] = 0x7f7f7f7f;
					buf[1] = 1200;					//2015.9.14 buf[1] = 1200+(EmptyNum<<16);
					buf[2] = i;	
					buf[3] = point;				//2015.9.14 new
					int len = sendtoPeer( (char *)buf, 1216 );
				}
			}
		};
		
};

int main()
{
	
	drvFPGA_Init();
	drvOCM_Init();
	tx aTx( "0.0.0.0", SATPORT );
	int point = drvFPGA_Read( BLOCK_CNT_OFFSET );
	int last_point = point;
	int rlast = (point-2);
	int cnt = 0;
	int flag = 0;
	drvOCM_Write( STOP_FLAG , flag );
	drvFPGA_Write( SAMPLE_RATE_OPT, 4 );
	drvFPGA_Write( RF_RING_REG, 1 );
	drvFPGA_Write( FPGA_RING_REG, 1);
	
	while( !flag )
	{
		point = drvFPGA_Read( BLOCK_CNT_OFFSET );
		int free = (point-2);
		int buflen = (free-rlast)&0x3f;
		if( buflen<60 )
		{
			if( rlast>free )
			{
					printf("index error\n");
		    		//aTx.read( rlast, 0x40 );
				    //aTx.read( 0, free );
			}
			else
		    {
				aTx.read( rlast, free );
			}
			if( rlast!=free ) 
			{
				cnt++;
				if( (cnt&0xfff)==0 )
				{
					printf("4096 time point = %x, det_point = %d, bufflen = %d\n",point, point-last_point, buflen );
					last_point = point;
				}
			}
			rlast = free;
			
		}
		else
		{
					printf("tx over flow\n");
		}
		flag = drvOCM_Read( STOP_FLAG );
	}
}


