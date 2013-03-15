#ifdef FUNCLASS

extern "C" {
	CCapability *newFun() {
		FUNCLASS *item = new FUNCLASS();
		return (CCapability*)item;
	}
	void run( CCapability* item, void *argv ) {
		(exampleFun1 *)item->run( argv );
	}
	void getKey( CCapability* item,long long int *k){
		(FUNCLASS *)item->getKey( k );
	}
	void destroy( CCapability* item ) {
		(FUNCLASS *)item->destroy();
	}
};	

#else
#error "Must define FUNCLASS Macro"
#endif
