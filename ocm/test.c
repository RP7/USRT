#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "ocm_io.h"
#include "fpga_io.h"

#define	RSTART_ADDER1		0x00000
#define	RSTART_ADDER2		0x01000
#define	WSTART_ADDER1		0x02000
#define	WSTART_ADDER2		0x03000
#define  RATE_SELECT		0x0000C014    //(0,1,2,3,4,5,6,7,8)120/240/480/960/1.92/3.84/7.68/15.36/30.72

#define  WH_ADDER		0xC020
#define  RH_ADDER 		0xC024
#define  NUM			300




int main()
{
	int i,j;
	drvOCM_Init();
	drvFPGA_Init();	
	int *wr_data[10*NUM] = {0};
	int*rd_data[10*NUM];
	int *wr_data1[NUM] = {0};
	int logic_w = 0;
	int reg_w,reg_r;
	int cnt[1000] = {0};
	for(i=0;i<10*NUM;i++)
	{
		wr_data[i]=i;	
	}

	for(i=0;i<10*NUM;i++)
	{
		wr_data1[i]=i;	
	}
drvFPGA_Write(RATE_SELECT, 4 );



while(1)
	{
				cnt[j]=drvFPGA_Read(  RH_ADDER );	
				reg_r = cnt[j]%600;
                               // for(i=0;i<1000;i++)
                              //  {
                               //         cnt[i] = drvFPGA_Read(  WH_ADDER );
                               // }
                              
                               //         printf("\n = %d\n",cnt[999]-cnt[0]);
                                

				if(reg_r>0&& reg_r<100)

					 {

						//printf("\n-reg_r = %d\n",reg_r);
 						drvOCM_WriteMem(  WSTART_ADDER2,  wr_data+j*NUM,  NUM );
						drvOCM_ReadMem(  RSTART_ADDER2,  rd_data,  NUM );

						for(i=0;i<30;i++)
							{
							 printf("  %d  ",rd_data[i]);
							}
						j = j+1	;

					//	printf("\n");
					}

				else if(reg_r>300 && reg_r<400)

					 {

					//	printf("\nreg_r = %d\n",reg_r);

						drvOCM_WriteMem(  WSTART_ADDER1,  wr_data+j*NUM,  NUM );
					//	printf("%d \n", wr_data1[10]);
						drvOCM_ReadMem(  RSTART_ADDER1,  rd_data,  NUM );
						for(i=0;i<30;i++)
							{
							 printf(" %d  ",rd_data[i]);
							}	
						//printf("\n");	
						j=j+1;	
					}

			if (j== 9)
			{
			 j=0;
			 printf("%d\n",cnt[9]=cnt[0]);
			 
			 
			}	
				

	}							
}	


