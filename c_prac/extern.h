#ifdef extern
#undef extern
#define EXTERN_FLAG
#endif

#include <stdio.h>

#ifdef EXTERN_FLAG
#define extern
#endif

extern int x
#ifdef extern
    =2
#endif 
     ;
