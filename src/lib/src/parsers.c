#include "include/api.h"

int sa3open_p (sa3cg* cg, const char* name) {
	int saret = 0;

	CHK_PTR (cg,   saret);
	CHK_CG  (*cg,  saret);
	CHK_STR (name, saret);

	CALL_API (saret, opencg (cg, name, DB_CREATE));

CLEANUP:
	return saret;
}

int sa3close_p (sa3cg* cg) {
	int saret = 0;

	CHK_PTR (cg,  saret);
	CHK_CG  (*cg, saret);

	CALL_API (saret, closecg (cg));

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
		CALL_API (saret, getkey ("dl \0", (const char*) _func, &keylen, &key));
		CALL_API (saret, put2db (cg->db, key, keylen, (void*) d, size));
	} /* TODO: child thread end */

CLEANUP:
	if (NULL != d) {
		freestruct (&d);
	}
	if (NULL != key) {
		freekey (&key);
	}

	return saret;
}

int sa3addcall (sa3cg cg, char* cr, char* cd) {
	/* Thread #0 (parser) */
	uint32   size = 0;	
	sa3call*    c = NULL;
	int     saret = 0;
	uint16 keylen = 0;
	char*      pk = NULL;
	char*      fk = NULL;
	char*     _cr = NULL;
	char*     _cd = NULL;

	CHK_CG  (cg, saret);
	CHK_STR (cr, saret);
	CHK_STR (cd, saret);

	CALL_API (saret, getstruct (&size, (void**)&c, 2, cr, cd));
	/* Thread #1 */
	_cr = (char*) PI2PTR (c, c->cr);
	_cd = (char*) PI2PTR (c, c->cd);

	CALL_API (saret, getkey ("cr \0", (const char*) _cr, &keylen, &pk));
	CALL_API (saret, put2db (cg->db, pk, keylen, (void*) c, size));

	CALL_API (saret, getkey ("cd \0", (const char*) _cd, &keylen, &fk));
	/* add FK */

	/* Thread #2 */
	CALL_API (saret, adddeps (cg, _cr, _cd));

CLEANUP:
	if (NULL != pk) {
		freekey (&pk);
	}
	if (NULL != fk) {
		freekey (&fk);
	}

	return saret;
}

int sa3mergedeps (sa3cg rcpt, sa3cg src) {
	int    saret = 0;
	sa3list  lst = NULL;
	uint32     i = 0;
	char*   pref = "cr \0";

	CHK_CG (rcpt, saret);
	CHK_CG (src,  saret);

	// FK: check out during testing whether length must be
	// strlen(pref) or strlen(pref)+1 (with null-term symbol).
	CALL_API(saret, getbypref (src->db, pref, strlen(pref) + 1, &lst));

	for (i = 0; i < lst->num; i++) {
		sa3call* call = (sa3call*)(lst->lst[i]);
		CALL_API(saret, adddeps (cg, call->cr, call->cd));
	}

CLEANUP:
	return saret;
}
