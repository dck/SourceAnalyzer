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

static int addent2lst (dep* src, dep* rcp) {
	uint32 i       = 0;
	int    ret     = 0;
	uint8  isExist = 0;
	int    saret   = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (src,    saret);
	CHK_PTR (rcp,    saret);
#endif

	for (i = 0; i < rcp->dpdsnum; i++) {
		if (src == rcp->dpds[i]) {
			isExist = 1;
			break;
		}
	}

	if (1 == isExist) {
		goto CLEANUP;
	}

	if (rcp->dpdsnum == rcp->bfsize) {
		rcp->bfsize += 10;
		CALL_API (saret, wrapmalloc (&(rcp->dpds), (rcp->bfsize) * sizeof (dep*)));
	}

	rcp->dpds[rcp->dpdsnum] = src;
	rcp->dpdsnum++;

CLEANUP:
	return saret;
}

static int addlst2lst (dep* srbase, dep* src, dep* rcbase, dep* rcp) {
	int saret = 0;
	uint32  i = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (srbase, saret);
	CHK_PTR (rcbase, saret);
	CHK_PTR (src,    saret);
	CHK_PTR (rcp,    saret);
#endif

	for (i = 0; i < src->dpdsnum; i++) {
		/* convert src to equiv in rcbase: (src - srcbase) + rcbase */
		dep* _src = (dep*)PI2PTR (rcbase, PTR2PI (srbase, src->dpds[i]));
		CALL_API (saret, addent2lst (_src, rcp));
	}

CLEANUP:
	return saret;
}

static int findlst (char* tofnd, dep prn[], uint32 num, dep** d) {
	int   saret = 0;
	uint32    i = 0;
	uint32 flen = strlen (tofnd);

#ifdef INTERNAL_PARAM_CHECKS
	CHK_STR (tofnd, saret);
	CHK_PTR (prn,   saret);
	CHK_PTR (d,     saret);
#endif
	(*d) = NULL;

	for (i = 0; i < num; i++) {
		if (flen == strlen(prn[i].func) &&
		    0 == strncmp(prn[i].func, tofnd, flen)) {
			(*d) = (prn + i);
			break;
		}
	}

CLEANUP:
	return saret;
}

static int addfnc(sa3cg cg, char* fn, rel r, dep** d) {
	int    saret = 0;
	size_t fnlen = 0;
	char*    _fn = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_CG  (cg, saret);
	CHK_STR (fn, saret);
	CHK_PTR (d,  saret);
#endif

	CALL_API (saret, wrapmalloc (&_fn, sizeof (char)*fnlen));
	strncpy(_fn, fn, fnlen);

	if (cg->bfsize == cg->fnnum) {
		cg->bfsize += 10;
		CALL_API (saret, wrapmalloc (&(cg->cld), (cg->bfsize)*sizeof (dep)));
		CALL_API (saret, wrapmalloc (&(cg->prn), (cg->bfsize)*sizeof (dep)));
	}

	cg->cld[cg->fnnum].fid     = cg->fnnum;	
	cg->cld[cg->fnnum].func    = _fn;
	cg->cld[cg->fnnum].flen    = fnlen;
	cg->cld[cg->fnnum].dpds    = NULL;
	cg->cld[cg->fnnum].dpdsnum = 0;
	cg->cld[cg->fnnum].bfsize  = 0;

	cg->prn[cg->fnnum].fid     = cg->fnnum;
	cg->prn[cg->fnnum].func    = _fn;
	cg->prn[cg->fnnum].flen    = fnlen;
	cg->prn[cg->fnnum].dpds    = NULL;
	cg->prn[cg->fnnum].dpdsnum = 0;
	cg->prn[cg->fnnum].bfsize  = 0;

	(cg->fnnum)++;

	if (prn == r) {
		(*d) = &(cg->prn[cg->fnnum-1]);
	} else {
		(*d) = &(cg->cld[cg->fnnum-1]);
	}

CLEANUP:
	return saret;
}

int adddeps (sa3cg cg, char* cr, char* cd) {
	dep* crprn = NULL;
	dep* crcld = NULL;
	dep* cdprn = NULL;
	dep* cdcld = NULL;
	uint32   i = 0;
	int  saret = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_CG  (cg, saret);
	CHK_STR (cr, saret);
	CHK_STR (cd, saret);
#endif

	CALL_API (saret, findlst (cr, cg->prn, cg->fnnum, &crprn));
	if (NULL == crprn) {
		CALL_API (saret, addfnc(cg, cr, prn, &crprn));
	}
	CALL_API (saret, findlst (cr, cg->cld, cg->fnnum, &crcld));
	if (NULL == crcld) {
		CALL_API (saret, addfnc(cg, cr, cld, &crcld));
	}
	CALL_API (saret, findlst (cd, cg->prn, cg->fnnum, &cdprn));
	if (NULL == cdprn) {
		CALL_API (saret, addfnc(cg, cd, prn, &cdprn));
	}
	CALL_API (saret, findlst (cd, cg->cld, cg->fnnum, &cdcld));
	if (NULL == cdcld) {
		CALL_API (saret, addfnc(cg, cd, cld, &cdcld));
	}
	CALL_API (saret, addlst2lst (NULL, crprn, NULL, cdprn));
	CALL_API (saret, addent2lst (crprn, cdprn));

	CALL_API (saret, addlst2lst (NULL, cdcld, NULL, crcld));
	CALL_API (saret, addent2lst (cdcld, crcld));

	for (i = 0; i < cdcld->dpdsnum; i++) {
		CALL_API (saret, addlst2lst (cg->prn, cdprn, cg->cld, cdcld->dpds[i]));
	}

	for (i = 0; i < crprn->dpdsnum; i++) {
		CALL_API (saret, addlst2lst (cg->cld, crcld, cg->prn, crprn->dpds[i]));
	}

CLEANUP:
	/* TODO: add clean up there */

	return ret;
}

int opencg (sa3cg* cg, const char* name, u_int32_t flags) {
	DB *dbp = NULL;    /* DB structure handle */

	int ret;           /* DB function return value */
	int saret = 0;     /* SA function return value */
	char* errmsg;      /* Error message */
	sa3cg _cg  = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (cg, saret);
	CHK_STR (name, saret);
#endif

	CALL_API (saret, wrapmalloc (&_cg, sizeof (struct cg)));
	CALL_API (saret, opendb(&dbp, flags));

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

static int freedep (dep** ppdep) {
	int saret = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (ppdep, saret);
#endif
	
	if (NULL != (*ppdep)) {
		dep* pdep = (*ppdep);
		if (NULL != pdep->dpds) {
			wrapfree(&(pdep->dpds));
		}
		wrapfree(&pdep);
		(*ppdep) = pdep;
	}

CLEANUP:
	return saret;
}

int closecg (sa3cg* cg) {
	int saret = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (cg,  saret);
	CHK_CG  (*cg, saret);
#endif

	CALL_API (saret, closedb (*cg));

	for (i = 0; i < (*cg)->fnnum; i++) {
		if (NULL != (*cg)->cld) {
			CALL_API (saret, freedep (&((*cg)->cld[i])));
		}
		if (NULL != (*cg)->prn) {
			CALL_API (saret, freedep (&((*cg)->prn[i])));
		}
	}
	
	if (NULL != (*cg)->cld) {
		wrapfree (&((*cg)->cld));
	}
	if (NULL != (*cg)->prn) {
		wrapfree (&((*cg)->prn));
	}
	wrapfree ((void**)cg);

CLEANUP:
	return saret;
}
