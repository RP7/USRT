extern "C" {
  int md5key(int64 key[2], char encrypt[]);
  int64 md5first(char encrypt[]);
  int64 md5second(char encrypt[]);
  int md5buf( unsigned char buf[], int len, int mode ); //mode=0 check, mode=1 gen
}