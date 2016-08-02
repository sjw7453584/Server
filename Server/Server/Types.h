/* Types.h
 *
 */

#ifndef _SERVER_TYPES_H
#define _SERVER_TYPES_H

#include <stdlib.h>
typedef long long	int64;

typedef int        int32;
typedef short      int16;
typedef char       int8;


typedef unsigned long long  uint64;

typedef unsigned int        uint32;
typedef unsigned short       uint16;
typedef unsigned char        uint8;

#ifndef WIN32
typedef unsigned int DWORD;
typedef int SOCKET;
#define	INVALID_SOCKET (-1)

#define atol(a) strtoul( a, NULL, 10)

#endif

#ifdef _WINDOWS

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define atoll _atoi64
#define strtoull _strtoui64
#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifdef WIN32
#ifndef bzero
#define bzero(a,b) memset(a, 0, b)
#endif
#endif




#endif
