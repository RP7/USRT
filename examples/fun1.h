#include<capability.h>
class exampleFun1 : CCapability {
	private:
		static long long int key[2];
	public:
		int getKey( long long int *k );
		int run( void *argv );
		int destroy();
};
	


