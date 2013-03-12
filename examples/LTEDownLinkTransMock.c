#include <usrttype.h>

#include"LTEDownLinkTransMock.h"
void *dummyArgv()
{
	return (void *)0;	
}
void *buildPss()
{
	return dummyArgv();
}
void *buildPssAntMap()
{
	return dummyArgv();
}
void *buildUeDownCoding(int i)
{
	return dummyArgv();
}
void *buildUeDownMod(int i)
{
	return dummyArgv();
}
void *buildUeDownAntMap(int i)
{
	return dummyArgv();
}
int64 keyPss()
{
	return 1;	
}
int64 keyPssAntMap()
{
	return 2;	
}
int64 keyUeDownCoding()
{
	return 3;	
}
int64 keyUeDownMod()
{
	return 4;	
}
int64 keyUeDownAntMap()
{
	return 5;	
}
