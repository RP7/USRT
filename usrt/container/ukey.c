#include <usrttype.h>

int64 key2int( ukey_t *key )
{
	return *(int64 *)(key);
}
int64 newKey( const char*name, int session, short *sn )
{
	assert( strlen(name)==3 );
	ukey_t v = { "", (*sn)++, session };
	strncpy( v.name,name,3 );
	v.name[3]='\0';
	return key2int( &v );	
}
int64 newE( const char*name, int session )
{
	return newKey( name, session, &(globe.snE) );	
} 
int64 newV( const char*name, int session )
{
	return newKey( name, session, &(globe.snV) );	
} 
char *nameOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (char *)(void *)&(pk->name); 
}
int snOfKey( int64 *key )
{
	ukey_t *pk=(ukey_t*)key;
	return (int)(pk->sn); 
}
