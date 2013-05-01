#ifdef FUNCLASS
#include <usrttype.h>
#include <capability.h>
#include <md5api.h>

class FUNCLASS : CCapability {
	private:
		static long long int key;
	public:
		long long int getKey( );
		int run( void *argv );
		int destroy();
};

extern "C" {
	CCapability *newFun() {
		FUNCLASS *item = new FUNCLASS();
		return (CCapability*)item;
	}
	void run( CCapability* item, void *argv ) {
		(FUNCLASS *)item->run( argv );
	}
	void getKey( CCapability* item,long long int *k){
		(FUNCLASS *)item->getKey( k );
	}
	void destroy( CCapability* item ) {
		(FUNCLASS *)item->destroy();
	}
};	
#define _STR(x) #x
#define STR(x) _STR(x)

long long int FUNCLASS::key = md5first(STR(FUNCLASS));
long long int FUNCLASS::getKey( ) {
	return key;
}
int FUNCLASS::destroy( ) {
	delete this;
	return 1;
}

#else
#error "Must define FUNCLASS Macro"
#endif
