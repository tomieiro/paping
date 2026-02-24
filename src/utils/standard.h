#ifndef PAPING_UTILS_STANDARD_H
#define PAPING_UTILS_STANDARD_H

#if defined(_WIN32) && !defined(WIN32)
#define WIN32 1
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#define VERSION "1.5.5"
#define AUTHOR "Mike Lovell"
#define YEAR ((((__DATE__[7] - '0') * 10 + (__DATE__[8] - '0')) * 10 + (__DATE__[9] - '0')) * 10 + (__DATE__[10] - '0'))

#ifdef WIN32
#define close closesocket
#define snprintf _snprintf
#endif

/* Error codes */
#define SUCCESS 0
#define ERROR_POUTOFMEMORY 100
#define ERROR_SOCKET_CANNOTRESOLVE 101
#define ERROR_SOCKET_TIMEOUT 102
#define ERROR_SOCKET_GENERALFAILURE 103
#define ERROR_INVALIDARGUMENTS 200

#include <errno.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32 /* Windows specific */
#include <WinSock2.h>
#include <Windows.h>
#include <time.h>
#else /* Linux/Unix specific */
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/* Types */
typedef const wchar_t *pcw_t;
typedef const char *pcc_t;
typedef wchar_t *pwc_t;
typedef char *pc_t;
typedef unsigned short ushort_t;

#endif /* PAPING_UTILS_STANDARD_H */

