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
	if (n < 0)
		in = in + n;
	for (i = 0; i < in; i++)
		fprintf (stderr, "  ");
	if (n > 0)
		in = in + n;
}
#endif

int open_cg (sa3cg* cg, const char* name) {
	database_t *dbp  = NULL;
	int   saret      = 0;
	sa3cg _cg        = NULL;

	CHK_I_PTR (cg, saret);
	CHK_I_STR (name, saret);

	CALL_API (saret, wrapmalloc (&_cg, sizeof (struct cg)));
    CALL_API (saret, open_db (&dbp, name));

    _cg->pdb  = dbp;
    (*cg) = _cg;

CLEANUP:
	if (0 != saret) {
		if (NULL != dbp)
			close_db (&dbp);
        if (NULL != _cg)
			wrapfree (&_cg);
	}

	return saret;
}

int create_cg (sa3cg* cg, const char* name) {
	database_t *dbp  = NULL;
	int   saret      = 0;
	sa3cg _cg        = NULL;

	CHK_I_PTR (cg, saret);
	CHK_I_STR (name, saret);

	CALL_API (saret, wrapmalloc (&_cg, sizeof (struct cg)));
    CALL_API (saret, create_db (&dbp, name));

    _cg->pdb  = dbp;
    (*cg) = _cg;

CLEANUP:
	if (0 != saret) {
		if (NULL != dbp)
			close_db (&dbp);
        if (NULL != _cg)
			wrapfree (&_cg);
	}

	return saret;
}

int close_cg (sa3cg* pcg) {
	int saret = 0;
	int i = 0;
	sa3cg cg = NULL;

	CHK_I_PTR (pcg,  saret);
	CHK_I_CG  (*pcg, saret);

	cg = *pcg;
	CALL_API (saret, close_db (&cg->pdb));

	wrapfree (pcg);

CLEANUP:

	return saret;
}

int addcld (sa3cg cg, char* func, char* initfunc) {
	int         saret       = 0;
	uint16      funclen     = 0;
	uint16      initfunclen = 0;
	sa3list     l    = NULL;
	sa3listit   it   = NULL;
	uint8       flag = 0;
	database_t* db   = NULL;

	// add params checks

	db = cg->pdb;

	// fprintf(stderr, "--> %s\n", func);

	funclen     = strlen(func) + 1;
	initfunclen = strlen(initfunc) + 1;

	CALL_API (saret, get_ent (db, func, funclen, &l, CHD));

	if (NULL == l) {
		CALL_API (saret, get_ent (db, func, funclen, &l, CR));
		SA3_FORLST(it, l) {
			char*    cd = NULL;
			sa3call* c  = NULL;

			c  = (sa3call*)sa3resolveit (l, it);
			cd = (char*)PI2PTR(c, c->cd);

			CALL_API (saret, is_ent_exist (db, initfunc, initfunclen, cd, c->cdlen, CHD, &flag));
			CALL_API (saret, put_ent (db, initfunc, initfunclen, cd, c->cdlen, CHD));

			if (0 == flag)
				CALL_API (saret, addcld (cg, cd, initfunc));
		}
	} else if (func != initfunc) {
		SA3_FORLST(it, l) {
			char* fn = (char*)sa3resolveit (l, it);
			CALL_API (saret, put_ent (db, initfunc, initfunclen, fn, strlen(fn) + 1, CHD));
		}
	}

CLEANUP:

	if (NULL != l)
		sa3freelist(&l);
	return saret;
}

int addprn (sa3cg cg, char* func, char* initfunc) {
	int         saret       = 0;
	uint16      funclen     = 0;
	uint16      initfunclen = 0;
	sa3list     l      = NULL;
	sa3listit   it     = NULL;
	uint8       flag   = 0;
	database_t* db     = NULL;

	// add params checks

	db = cg->pdb;

	// fprintf(stderr, "--> %s\n", func);

	funclen     = strlen(func) + 1;
	initfunclen = strlen(initfunc) + 1;

	CALL_API (saret, get_ent (db, func, funclen, &l, PRN));

	if (NULL == l) {
		CALL_API (saret, get_ent (db, func, funclen, &l, CD));
		SA3_FORLST(it, l) {
			char*    cr = NULL;
			sa3call* c  = NULL;

			c  = (sa3call*) sa3resolveit (l, it);
			cr = (char*) PI2PTR(c, c->cr);

			CALL_API (saret, is_ent_exist (db, initfunc, initfunclen, cr, c->crlen, PRN, &flag));
			CALL_API (saret, put_ent (db, initfunc, initfunclen, cr, c->crlen, PRN));

			if (0 == flag)
				CALL_API (saret, addprn (cg, cr, initfunc));
		}
	} else if (func != initfunc) {
		SA3_FORLST(it, l) {
			char* fn = (char*)sa3resolveit (l, it);
			CALL_API (saret, put_ent (db, initfunc, initfunclen, fn, strlen(fn) + 1, PRN));
		}
	}

CLEANUP:

	if (NULL != l)
		sa3freelist(&l);
	return saret;
}
