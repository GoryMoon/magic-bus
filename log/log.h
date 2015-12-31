#include <stdio.h>


#ifdef LOG_TO_FILE
#define log(a) printf a ;
#else
#define log(a)
#endif

#ifdef DEBUG
#define debug(a) printf("[%s::%s():%d]: ", __FILE__, __func__, __LINE__); printf  a ;
#else
#define debug(a)
#endif

