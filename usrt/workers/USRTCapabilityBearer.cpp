#include <stdio.h>
#include <USRTCapabilityBearer.h>
#include <MapMem.h>

USRTCapabilityBearer::USRTCapabilityBearer( const char* lib )
{
  mVaild=0;
  if( getHandle(lib) ) {
    dlerror();    /* Clear any existing error */
    mVaild=1;
    init();
  }
}

int USRTCapabilityBearer::getHandle(const char* lib) 
{
  handle = dlopen (getFileName(lib), RTLD_LAZY);
  if (!handle) {
    fprintf (stderr, "load %s error : ", lib);
    fprintf (stderr, "%s\n", dlerror());
    return -1;
  }
  else
    return 0;
}
  
void *USRTCapabilityBearer::getFunc(const char *sym)
{
  void *func = dlsym(interface.handle, sym);
  if ((error = dlerror()) != NULL)  {
    fprintf (stderr, "%s\n", error);
    mVaild=0;
    return NULL;
  }
}

void USRTCapabilityBearer::init()
{
  mFactroy=getFunc("factroy");
  mRun=getFunc("run");
  mGetKey=getFunc("getKey");
  mDistroy=getFunc("distroy");
  if( factroy )
    item=mFactroy();
  else
    item=NULL;
  if( getKey )
    key=mGetKey(item);
  else
    key=0LL;
}

void USRTCapabilityBearer::run(generalized_memory_t* gpArgv)
{
  void *lpArgv = G2L( gpArgv );
  mRun(item,lpArgv);
}

USRTCapabilityBearer::~USRTCapabilityBearer()
{
  if (handle) {
    mDistroy(item);
    dlclose(handle);
  }
} 

const char *USRTCapabilityBearer::getWorkingDir (void) 
{
    char *workdir;

    if ((workdir = getenv ("USRTWORK")) != NULL)   return workdir;

    return "work";
}

const char *USRTCapabilityBearer::getFileName( const char *n)
{
  strcpy(mName,getWorkingDir());
  strcat(mName,"/");
  strcat(mName,n);
  return mName;
}

const char *USRTCapabilityBearer::findByKey( int64 key )
{
    struct structCPBMeta meta;
    if( fp !=NULL ) {
      fread(&meta,1,sizeof(struct structCPBMeta),fp);
      fclose(fp);
      fprintf(stderr," check file %s\n",entry->d_name);
      if( meta.key[0]==key ) {
        strcpy(mName,entry->d_name);
        fprintf(stderr," find file %s\n",mName);
        
        return mName;
      }
    }
  }
  closedir( dir );
  return NULL; 
}
   
USRTCapabilityBearer::USRTCapabilityBearer( int64 k )
{
  mVaild=0;
  const char *workdir = getWorkingDir();
  struct dirent* entry;
  DIR* dir = opendir(workdir);
  while( (entry=readdir(dir))!=NULL ) {
    handle = dlopen (getFileName(lib), RTLD_LAZY);
    if( !handle ) {
      fprintf(stderr," check lib %s fail\n",entry->d_name);
      continue;
    }
    mValid=1;
    init();
    if( key==k )
      break;
    mValid=0;
    dlclose(handle);
  }
}

};

#endif //Capability_Bearer_H

