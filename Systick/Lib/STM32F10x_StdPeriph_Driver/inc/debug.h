/* You must include this header file to use debug feature.*/
#ifndef	__DEBUG_H
#define	__DEBUG_H
#include <stdio.h>
/* Comment this line to remove all DBG.*/
#define	DEBUG

#ifdef DEBUG
	#define DBG(fmt,...) printf("%s:  %s():  %d: "fmt, __FILE__, __FUNCTION__, __LINE__,##__VA_ARGS__) 
#else
  #define DBG(fmt, ...)
#endif

#endif


