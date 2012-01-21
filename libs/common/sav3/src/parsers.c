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

int sa3open_p (sa3cg* cg, const char* name) {
	int saret = 0;

	CHK_PTR (cg,   saret);
	CHK_STR (name, saret);

	CALL_API (saret, create_cg (cg, name));

CLEANUP:

	return saret;
}

int sa3close_p (sa3cg* cg) {
	int saret = 0;

	CHK_PTR (cg,  saret);
	CHK_CG  (*cg, saret);

	CALL_API (saret, close_cg (cg));

CLEANUP:

	return saret;
}

int sa3adddecl (sa3cg cg, char* type, char* func, char* param) {
	uint32   size   = 0;
	sa3decl* d      = NULL;
	int      saret  = 0;
	uint16   keylen = 0;
	char*    key    = NULL;
	char*    _func  = NULL;

	CHK_CG  (cg,    saret);
	CHK_STR (type,  saret);
	CHK_STR (func,  saret);
	CHK_STR (param, saret);

	CALL_API (saret, getstruct (&size, (void**) &d, 3, type, func, param));
	
	/*   TODO: child thread start */ {
		_func = (char*) PI2PTR (d, d->func);
		// CALL_API (saret, getkey ("dl \0", (const char*) _func, &keylen, &key));
		CALL_API (saret, put_ent (cg->pdb, _func, STRLEN(_func), (void*) d, size, DL));
	} /* TODO: child thread end */

CLEANUP:
	if (NULL != d) {
		freestruct ((void**)&d);
	}
	if (NULL != key) {
		freekey (&key);
	}

	return saret;
}

int sa3addcall (sa3cg cg, char* cr, char* cd) {
	/* Thread #0 parser */
	uint32   size = 0;	
	sa3call*    c = NULL;
	int     saret = 0;
	uint16  pklen = 0;
	uint16  fklen = 0;
	char*      pk = NULL;
	char*   tmpcr = NULL;

	CHK_CG  (cg, saret);
	CHK_STR (cr, saret);
	CHK_STR (cd, saret);

	CALL_API (saret, getstruct (&size, (void**)&c, 2, cr, cd));

	CALL_API (saret, put_ent (cg->pdb, NULL, 0, (void*) c, size, CR));

CLEANUP:
	if (NULL != pk) {
		freekey (&pk);
	}

	return saret;
}

int sa3mergedecls (sa3cg rcpt, sa3cg src) {
	int    saret = 0;
	sa3list  lst = NULL;
	sa3listit it = NULL;
	char*   pref = "dl \0";

	CHK_CG (rcpt, saret);
	CHK_CG (src,  saret);

	CALL_API(saret, get_all_ent (src->pdb, &lst, DL));

	SA3_FORLST (it, lst) {
		char* type  = NULL;
		char* func  = NULL;
		char* param = NULL;
		sa3decl* d  = NULL;
		d = (sa3decl*)sa3resolveit (lst, it);
		type  = (char*)PI2PTR(d, d->type);
		func  = (char*)PI2PTR(d, d->func);
		param = (char*)PI2PTR(d, d->param);
		// no (NULL != call) check needed, since sa3adddecl() is a public
		// API and will do argumants check
		CALL_API(saret, sa3adddecl (rcpt, type, func, param));
	}

CLEANUP:
	if (NULL != lst)
		sa3freelist (&lst);
	return saret;
}

int sa3mergecalls (sa3cg rcpt, sa3cg src) {
	int    saret = 0;
	sa3list  lst = NULL;
	sa3listit it = NULL;

	CHK_CG (rcpt, saret);
	CHK_CG (src,  saret);

	CALL_API(saret, get_all_ent (src->pdb, &lst, CR));

	SA3_FORLST (it, lst) {
		char*   cr = NULL;
		char*   cd = NULL;
		sa3call* c = NULL;
		c = (sa3call*)sa3resolveit (lst, it);
		cr = (char*)PI2PTR(c, c->cr);
		cd = (char*)PI2PTR(c, c->cd);
		// no (NULL != call) check needed, since sa3adddecl() is a public
		// API and will do argumants check
		CALL_API(saret, sa3addcall (rcpt, cr, cd));
	}

CLEANUP:
	if (NULL != lst)
		sa3freelist (&lst);
	return saret;
}

/* About to exclude this stuff. Dependencies are calculated on demand.
int sa3mergedeps (sa3cg rcpt, sa3cg src) {
	int    saret = 0;
	sa3list  lst = NULL;
	char*   pref = "cr \0";

	CHK_CG (rcpt, saret);
	CHK_CG (src,  saret);

	CALL_API(saret, getbypref (src->db, pref, strlen(pref), &lst));

	{
	sa3listit it = NULL;
	SA3_FORLST (it, lst) {
		sa3call* call = (sa3call*)sa3resolveit (lst, it);
		// if (NULL != call)
		// CALL_API(saret, adddeps (rcpt, PI2PTR(call, call->cr), PI2PTR(call, call->cd)));
	}
	}

CLEANUP:
	if (NULL != lst)
		sa3freelist (&lst);
	return saret;
}
*/
