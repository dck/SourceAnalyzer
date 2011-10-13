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

#ifdef TRACE
	void __tabint(int n) {
		static uint16 in = 0;
		uint16 i = 0;
		if (0 > n) {
			in = in + n;
		//for (i = 0; i < in; i++)
			fprintf (stderr, "%d", in);
		} if (0 < n)
			in = in + n;
	}
#endif

int opencg (sa3cg* cg, const char* name, u_int32_t flags) {
	DB *dbp = NULL;    /* DB structure handle */
	int saret = 0;     /* SA function return value */
	sa3cg _cg  = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (cg, saret);
	CHK_STR (name, saret);
#endif

	CALL_API (saret, wrapmalloc (&_cg, sizeof (struct cg)));
	CALL_API (saret, opendb (&dbp, name, flags));

	_cg->db = dbp;

CLEANUP:
	if (0 != saret) {
		if (NULL != dbp) {
			dbp->close(dbp, 0);
		}
	} else {
		(*cg) = _cg;
	}

	return saret;
}

int closecg (sa3cg* pcg) {
	int saret = 0;
	int i = 0;
	sa3cg cg = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (pcg,  saret);
	CHK_CG  (*pcg, saret);
#endif

	cg = *pcg;
	CALL_API (saret, closedb (cg));

	wrapfree ((void**)pcg);

CLEANUP:
	return saret;
}

int addcld (DB* db, char* func, char* initfunc) {
	int saret = 0;
	uint16 kl    = 0;
	char*  k     = NULL;
	sa3list l    = NULL;
	sa3listit it = NULL;
	uint8 flag = 0;

	fprintf(stderr, "--> %s\n", func);
	// printf(".");
	CALL_API (saret, getkey ("cld \0", func, &kl, &k));
	CALL_API (saret, getbykey (db, k, kl, &l));

	if (NULL == l) {
		freekey(&k);
		CALL_API (saret, getkey ("cr \0", func, &kl, &k));
		CALL_API (saret, getbykey (db, k, kl, &l));
		freekey(&k);
		CALL_API (saret, getkey ("cld \0", initfunc, &kl, &k));
		SA3_FORLST(it, l) {
			char*   cd = NULL;
			sa3call* c = (sa3call*)sa3resolveit (l, it);
			cd = PI2PTR(c, c->cd);
			CALL_API (saret, ispairexists (db, k, kl, cd, c->cdlen, &flag));
			CALL_API (saret, put2db (db, k, kl, cd, c->cdlen));
			if (0 == flag) CALL_API (saret, addcld (db, cd, initfunc));
		}
	} else if (func != initfunc) {
		freekey(&k);
		CALL_API (saret, getkey ("cld \0", initfunc, &kl, &k));
		SA3_FORLST(it, l) {
			char* fn = (char*)sa3resolveit (l, it);
			CALL_API (saret, put2db(db, k, kl, fn, strlen(fn)+1));
		}
	}

CLEANUP:
	if (NULL != k)
		freekey(&k);
	return saret;
}

int addprn (DB* db, char* func, char* initfunc) {
	int saret = 0;
	uint16 kl    = 0;
	char*  k     = NULL;
	sa3list l    = NULL;
	sa3listit it = NULL;
	uint8   flag = 0;

	fprintf(stderr, "--> %s\n", func);
	CALL_API (saret, getkey ("prn \0", func, &kl, &k));
	CALL_API (saret, getbykey (db, k, kl, &l));

	if (NULL == l) {
		freekey(&k);
		CALL_API (saret, getseckey (db, "cd \0", func, &kl, &k));
		if (NULL != k) {
			CALL_API (saret, getbykey (db, k, kl, &l));
			CALL_API (saret, getkey ("prn \0", initfunc, &kl, &k));
			SA3_FORLST(it, l) {
				char*   cr = NULL;
				sa3call* c = (sa3call*)sa3resolveit (l, it);
				cr = PI2PTR(c, c->cr);
				CALL_API (saret, ispairexists (db, k, kl, cr, c->crlen, &flag));
				CALL_API (saret, put2db (db, k, kl, cr, c->crlen));
				if (0 == flag) CALL_API (saret, addprn (db, cr, initfunc));
			}
		}
	} else if (func != initfunc) {
		freekey(&k);
		CALL_API (saret, getkey ("prn \0", initfunc, &kl, &k));
		SA3_FORLST(it, l) {
			char* fn = (char*)sa3resolveit (l, it);
			CALL_API (saret, put2db(db, k, kl, fn, strlen(fn)+1));
		}
	}

CLEANUP:
	if (NULL != k)
		freekey(&k);
	if (NULL != l)
		sa3freelist(&l);
	return saret;
}
