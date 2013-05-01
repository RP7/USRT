#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usrttype.h>
#include "md5.h"

extern "C" {
int md5key(int64 key[2], char encrypt[])
{  
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    unsigned char decrypt[16];
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(&md5,decrypt);
    unsigned char *reverse = (unsigned char *)key;
    for( i=0;i<16;i++ )
    	reverse[i]=decrypt[15-i];
    return 0;
}

int64 md5first(char encrypt[])
{
	int64 key[2];
	md5key( key, encrypt );
	return key[0];	
}

int64 md5second(char encrypt[])
{
	int64 key[2];
	md5key( key, encrypt );
	return key[1];	
}

int md5keybuf(int64 key[2], unsigned char buf[], int len)
{  
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    unsigned char decrypt[16];
    MD5Update(&md5,buf,len);
    MD5Final(&md5,decrypt);
    unsigned char *reverse = (unsigned char *)key;
    for( i=0;i<16;i++ )
    	reverse[i]=decrypt[15-i];
    return 0;
}

int md5buf( unsigned char buf[], int len, int mode )
{
  int64* key=(int64*)(buf+len-16);
  int64 _md5[2];
  md5keybuf( _md5, buf, len-16 );
  if( mode==0 ) {
    if( key[0]==_md5[0] && key[1]==_md5[1] )
      return 0;
    else
      return 1;
  }
  else {
    key[0]=_md5[0];
    key[1]=_md5[1];
    return 1;
  }
}
}