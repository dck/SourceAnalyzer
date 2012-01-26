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

int getkey (const char* pref, const char* val, uint16* keylen, char** pkey) {
	int saret = 0;
	uint16 preflen = 0;
	uint16 vallen = 0;
	char* key = NULL;

	CHK_I_STR (pref,   saret);
	CHK_I_STR (val,    saret);
	CHK_I_PTR (keylen, saret);
	CHK_I_PTR (pkey,   saret);

	preflen   = strlen(pref);
	vallen    = strlen(val) + 1;
	(*keylen) = preflen + vallen;
	CALL_API (saret, wrapmalloc (&key, *keylen));
	strncpy (key, pref, preflen);
	strncpy (key + preflen, val, vallen);

CLEANUP:
	if (0 != saret && NULL != key) {
		wrapfree (&key);
	} else {
		(*pkey) = key;
	}

	return saret;
}

#ifdef TRACE
static void dump_sa3call(sa3call* c, char* pref) {
	char* cr;
	char* cd;
	cr = (char*)PI2PTR (c, c->cr);
	cd = (char*)PI2PTR (c, c->cd);
	fprintf(stderr, "struct addr: 0x%x; will add %s: cr: %s (%d), cd: %s (%d)",
		(unsigned int) c, pref, cr, c->crlen, cd, c->cdlen);
}
#endif

static int genfk(DB *fdb, const DBT *pkey, const DBT *data, DBT *fkey, char* fkey_str) {
	memset(fkey, 0, sizeof(DBT));
	fkey->data = fkey_str;
	fkey->size = strlen(fkey_str) + 1;
#ifdef TRACE
	fprintf(stderr, "; FK: %s (%d)\n", (char*)fkey->data, fkey->size);
#endif
	// not sure how to fail correctly, so call-back is fail-proof
	// with assumption that correct data is sent via arguments
	return 0;	
}

int  gen_cr (DB *fdb, const DBT *pkey, const DBT *data, DBT *fkey) {
	sa3call* c = (sa3call*)data->data;
#ifdef TRACE
	dump_sa3call(c, "cr");
#endif
	return genfk(fdb, pkey, data, fkey, (char*)PI2PTR (c, c->cr));
}

int  gen_cd (DB *fdb, const DBT *pkey, const DBT *data, DBT *fkey) {
	sa3call* c = (sa3call*)data->data;

#ifdef TRACE
	fprintf(stderr, "\n");
	dump_sa3call(c, "cd");
#endif

	return genfk(fdb, pkey, data, fkey, (char*)PI2PTR (c, c->cd));
}

void freekey (char** pkey) {
	wrapfree(pkey);
}

int getstruct (uint32* size, void** struc, uint8 cnt, ...) {
	int     saret     = 0;
	uint8   i         = 0;
	char*   str       = NULL;
	uint32  shiftData = 0;
	uint8*  buf       = NULL;
	uint16* pArgLen   = NULL;

	CHK_I_NOTZERO (cnt,   saret);
	CHK_I_PTR     (struc, saret);

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

