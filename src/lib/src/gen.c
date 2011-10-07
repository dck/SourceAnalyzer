#include "include/api.h"

int getkey (const char* pref, const char* val, uint16* keylen, char** pkey) {
        int saret = 0;
        uint16 preflen = 0;
        uint16 vallen = 0;
        char* key = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_STR (pref,   saret);
	CHK_STR (val,    saret);
	CHK_PTR (keylen, saret);
	CHK_PTR (pkey,   saret);
#endif

	preflen   = strlen(pref);
	vallen    = strlen(val) + 1;
        (*keylen) = preflen + vallen;
	CALL_API (saret, wrapmalloc (&key, *keylen));
        strncpy (key, pref, preflen);
        strncpy (key + preflen, val, vallen);
        strncpy (key + (*keylen), "\0", 1);

CLEANUP:
	if (0 != saret && NULL != key) {
		wrapfree (&key);
	} else {
		(*pkey) = key;
	}

        return saret;
}

void freekey (char** pkey) {
	wrapfree(pKey);
}

int getstruct (uint32* size, void** struc, uint8 cnt, ...) {
	int     saret     = 0;
	uint8   i         = 0;
	char*   str       = NULL;
	uint32  shiftData = 0;
	uint8*  buf       = NULL;
	uint16* pArgLen   = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_NOTZERO (cnt,   saret);
	CHK_PTR     (struc, saret);
#endif

	CALL_API (saret, wrapmalloc (&pArgLen, sizeof(uint16)*cnt));

        (*size) = cnt * sizeof(struct model);        
        shiftData = (*size);
        
        FOREACH_ARG_BEGIN(i, cnt, str) {
                pArgLen[i] = strlen(str)+1;
                (*size) += pArgLen[i];
        } FOREACH_ARG_END
        
	CALL_API (saret, wrapmalloc(&buf, (*size)));

        FOREACH_ARG_BEGIN(i, cnt, str) {
                uint8*  pair   = NULL;
                char*   pArg   = NULL;
                uint16* pArgSh = NULL;
                uint16* pLen   = NULL;
                
                pair = PI2PTR (buf, i*sizeof(struct model));
                pArg = (char*) PI2PTR (buf, shiftData);
                
                strncpy(pArg, str, pArgLen[i]);

                pArgSh  = (uint16*) PI2PTR (pair, 0);
                *pArgSh = shiftData;

                pLen  = (uint16*) PI2PTR(pair, sizeof(char*));
                *pLen = pArgLen[i];

                shiftData += pArgLen[i];
        } FOREACH_ARG_END

	(*struc) = buf;

CLEANUP:
	if (NULL != pArgLen) {
		wrapfree(&pArgLen);
	}

        return saret;
}

void freestruct (void** struc) {
	wrapfree(struc);
}

