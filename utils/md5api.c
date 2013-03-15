#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usrttype.h>
#include "md5.h"

int md5key(int64 key[2], char encrypt[])
{  
    MD5_CTX md5;
    MD5Init(&md5);
    int i;
    unsigned char *decrypt = (unsigned char *)key;
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(&md5,decrypt);
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
