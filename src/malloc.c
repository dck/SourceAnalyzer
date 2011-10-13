// Copyright 2008-2011 SourceAnalyzer team. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY SOURCEANALYZER TEAM ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SOURCEANALYZER TEAM OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of SourceAnalyzer team.

#include "include/api.h"

#define BUF2SIZE(ptr)   (size_t*) PTR2PI (sizeof(size_t), ptr)
#define SIZE2BUF(ptr)   (void*)   PI2PTR (ptr, sizeof(size_t))
#define BOUND_BYTE  (0xFF)

#ifdef MALLOC_CHECKS
	// Extra byte for buffer boundary marker
	#define ALLOCSIZE(len)  ((size_t)(len) + sizeof(size_t) + 1)
#else
	#define ALLOCSIZE(len)  ((size_t)(len) + sizeof(size_t))
#endif

#ifdef MALLOC_CHECKS

void printbuf (void* ptr) {
	uint32 i = 0;
	void* bp = BUF2SIZE(ptr);
	fprintf(stderr, "ptr:  %x; base: %x; Memory (base): ", (uint32)ptr, (uint32)bp);
	for(i = 0; i < (*(size_t*)bp) + sizeof(size_t) + 1; i++)
		fprintf(stderr, "0x%x ", *PI2PTR(bp, i));
	fprintf(stderr, "\n");
}

int checkboundaries (void* ptr) {
	int     saret = 0;
	size_t* psize = NULL;
#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (ptr, saret);
	CHK_GTZERO (*BUF2SIZE(ptr), saret)
#endif
	psize = BUF2SIZE(ptr);
	GOTO_CLEANUP_IF(BOUND_BYTE != *PI2PTR(ptr, *psize), saret, SA3_MALLOC_ERR);

CLEANUP:
	if (SA3_MALLOC_ERR == saret) {
		fprintf (stderr, "BUFFER OVERRUN: ");
		printbuf (ptr);
		exit(SA3_MALLOC_ERR);
	}

	return saret;
}
#endif

int _wrapmalloc (void** pptr, size_t len) {
	void*         tmp = NULL;
	int         saret = 0;
	size_t     oldlen = 0;
	size_t*  arraylen = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (pptr, saret);
	CHK_GTZERO (len, saret);
#endif

	tmp = *pptr;
	if( NULL != tmp ) {
		tmp    = BUF2SIZE (tmp);
		oldlen = *((size_t*)tmp);
#ifdef MALLOC_CHECKS
		CALL_API (saret, checkboundaries(*pptr));
#endif
	}

	tmp = realloc (tmp, ALLOCSIZE(len));
	GOTO_CLEANUP_IF(NULL == tmp, saret, SA3_MALLOC_ERR);

	arraylen = (size_t*) tmp;
	tmp = SIZE2BUF(tmp);
	if (len > oldlen) {
		memset (PI2PTR(tmp, oldlen), 0x0, (len - oldlen));
	}
	*arraylen = len;
#ifdef MALLOC_CHECKS
	// setting boundary for the buffer
	memset (PI2PTR(tmp, len), BOUND_BYTE, 1);
	fprintf (stderr, "BUFFER ALLOCATED: ");
	printbuf (tmp);
#endif
	*pptr = tmp;
CLEANUP:

	return saret;
}

void _wrapfree (void** pptr) {
#if defined(INTERNAL_PARAM_CHECKS) || defined(MALLOC_CHECKS)
	int saret = 0;
#endif

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (pptr, saret);
	CHK_PTR (*pptr, saret);
#endif
	{
		void* ptr = BUF2SIZE (*pptr);
#ifdef MALLOC_CHECKS
		CALL_API (saret, checkboundaries(*pptr));
		fprintf (stderr, "BUFFER TO FREE: ");
		printbuf (*pptr);
		memset(ptr, 0xAA, *(size_t*)(ptr));
#endif

		free (ptr);
		*pptr = NULL;
	}
#if defined(INTERNAL_PARAM_CHECKS) || defined(MALLOC_CHECKS)
CLEANUP:
	fprintf(stderr, "warning: no status is returned from wrapfree()\n");
#endif
}

size_t getbufsize(void* ptr) {
	return *(BUF2SIZE(ptr));
}
