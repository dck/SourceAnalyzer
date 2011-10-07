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

