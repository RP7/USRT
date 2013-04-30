#include <usrttype.h>

extern "C" {
  int64 key2int( ukey_t *key );
  int64 newKey( const char*name, int session, short *sn );
  int64 newE( const char*name, int session );
  int64 newV( const char*name, int session );
  char *nameOfKey( int64 *key );
  int snOfKey( int64 *key );
};
