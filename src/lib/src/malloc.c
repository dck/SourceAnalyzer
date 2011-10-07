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

#define BUF2SIZE     (ptr)   (size_t*) PTR2PI (sizeof(size_t), ptr)
#define SIZE2BUF     (ptr)   (uint8*)  PI2PTR (ptr, sizeof(size_t))
#define BOUND_BYTE  (0xFF)

#ifdef MALLOC_CHECKS
	// Extra byte for buffer boundary marker
	#define ALLOCSIZE (len)  ((size_t)(len) + sizeof(size_t) + 1)
#else
	#define ALLOCSIZE (len)  ((size_t)(len) + sizeof(size_t))
#endif

#ifdef MALLOC_CHECKS
int checkboundaries (void* ptr) {
	int   saret = 0;
	size_t size = 0;
#ifdef INTERNAL_PARAM_CHECKS
	EXIT_IF_NULLPTR (ptr, saret);
	EXIT_IF (0 >= *BUF2SIZE(ptr), saret, SA3_PARAM_ERR);
#endif
	size = *BUFSIZE(ptr);
	if (BOUND_BYTE != PI2PTR(ptr, size)) {
		log (error, "buffer is corrupt");
		ret = SA3_INTERNAL_ERR;
		goto CLEANUP;
	}

CLEANUP:
	return saret;
}
#endif

int _wrapmalloc (void** pptr, size_t len) {
	void*         tmp = NULL;
	int         saret = 0;
	size_t*  arraylen = 0;

#ifdef INTERNAL_PARAM_CHECKS
#endif

	tmp = *pptr;
	if( NULL != tmp ) {
		tmp = BUF2SIZE (tmp);
#ifdef MALLOC_CHECKS
		EXIT_IF(0 == (*tmp), saret, SA3_PARAM_ERR);
		saret = checkboundaries(*pptr);
		EXIT_IF_SAFAIL (saret);
		len++;
#endif
	}

	tmp = realloc (tmp, ALLOCSIZE(len));
	if (NULL == tmp) {
		log (error, "memory allocation failed");
		ret = SA3_MALLOC_ERR;
		goto CLEANUP;
	}

	arraylen = (size_t*) tmp;
	tmp = SIZE2BUF(tmp);
	if (len < (*arraylen)) {
		memset (PI2PTR(tmp, *arraylen), 0x0, (len - *arraylen));
	}
	*arraylen = len;
#ifdef MALLOC_CHECKS
	// setting boundary for the buffer
	memset (PI2PTR(tmp, *arraylen), BOUND_BYTE, 1);
#endif
	*pptr = tmp;
CLEANUP:
	return saret;
}

void _wrapfree (void** pptr) {
#ifdef INTERNAL_PARAM_CHECKS
	int saret = 0;
	EXIT_IF_NULLPTR (pptr,  saret); 
	EXIT_IF_NULLPTR (*pptr, saret);
#endif
	{
		uint8* ptr = BUF2SIZE (*pptr);
#ifdef MALLOC_CHECKS
		EXIT_IF(0 == (*ptr), saret, SA3_PARAM_ERR);
		saret = checkboundaries (*pptr);
		EXIT_IF_SAFAIL (saret);
#endif
		free (ptr);
		*pptr = NULL;
	}
CLEANUP:
}

