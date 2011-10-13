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

int sa3open_t     (sa3cg* cg, const char* name) {
	int saret = 0;

	CHK_PTR (cg,   saret);
	CHK_STR (name, saret);

	CALL_API(saret, opencg (cg, name, 0 /*DB_RDONLY*/));

CLEANUP:
	return saret;
}

int sa3close_t    (sa3cg* cg) {
	int saret = 0;

	CHK_PTR (cg,  saret);
	CHK_CG  (*cg, saret);

	CALL_API(saret, closecg (cg));

CLEANUP:
	return saret;
}

void      sa3freelist   (sa3list* plist) {
	if (NULL != plist) {
		sa3list list = *plist;
		if (NULL != list) {
			if (NULL != list->cur) {
				list->cur->close (list->cur);
				list->cur = NULL;
				if (NULL != list->lst) {
					uint32 i = 0;
					for (i = 0; i < list->num; i++)
						if (NULL != list->lst[i])
							wrapfree (&(list->lst[i]));
					wrapfree (&(list->lst));
				}
				wrapfree (&list);
			}
			list = NULL;
		}
		*plist = list;
	}
}

sa3listit sa3firstlstit (sa3list lst) {
	sa3listit it = NULL;
	if (NULL == lst || NULL != lst->cur) {
		it = (sa3listit)0;
	} else { // obtained thru sa3getclds() or sa3getprns() call.
		it = (sa3listit)PI2PTR(lst, lst->lst);
	}
	return it;
}

sa3listit sa3nextlstit  (sa3list lst, sa3listit it) {
	if (NULL == lst)
		return NULL;
	if (NULL != lst->cur) {
		it = (sa3listit)PI2PTR(it, 1);
	} else {
		it = (sa3listit)PI2PTR(it, sizeof(char*));
	}
	return it;
}

sa3listit sa3lastlstit  (sa3list lst) {
	sa3listit it = NULL;
	if (NULL == lst)
		return NULL;
	if (NULL != lst->cur) {
		it = (sa3listit)lst->num;
	} else {
		it = (sa3listit)PI2PTR (lst, lst->lst);
		it = (sa3listit)PI2PTR (it, sizeof(char*) * lst->num);
	}
	return it;
}

void*     sa3resolveit  (sa3list lst, sa3listit it) {
	void* ptr = NULL;
	if (NULL == lst)
		return NULL;
	if (NULL != lst->cur) {
		ptr = lst->lst[(size_t)it];
	} else { // obtained thru sa3getclds() or sa3getprns() call.
		ptr = (void*)PI2PTR(lst, *it);
	}

	return (void*)ptr;
}

int sa3getfuncs   (sa3cg cg, sa3list* plst) {
	int    saret = 0;
	sa3list  lst = NULL;
	uint32     i = 0;
	char*   pref = "dl \0";

	CHK_CG  (cg,   saret);
	CHK_PTR (plst, saret);

	CALL_API(saret, getbypref (cg->db, pref, strlen(pref), &lst));

	{
		sa3listit it = NULL;
		for (i = 0; i < lst->num; i++) {
			sa3decl* decl = (sa3decl*)lst->lst[i];
			// if (NULL != call)
			lst->lst[i] = PI2PTR(decl, decl->func);
		}
	}

	*plst = lst;
CLEANUP:
	if (0 != saret && NULL != lst) {
		sa3freelist(&lst);
	}
	return saret;
}

int sa3getdecls   (sa3cg cg, char* func, sa3list* list) {
	int       saret = 0;
	uint16    klen  = 0;
	char*     key   = NULL;
	sa3listit i     = NULL;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);

	CALL_API(saret, getkey ("dl \0", func, &klen, &key));
	CALL_API(saret, getbykey (cg->db, key, klen, list));

	if (NULL != list && NULL != (*list))
		for (i  = sa3firstlstit (*list);
		     i != sa3lastlstit  (*list);
		     i  = sa3nextlstit  (*list, i)) {
			sa3decl* decl = (sa3decl*)sa3resolveit(*list, i);
			decl->func  = (char*) PI2PTR(decl, decl->func);
			decl->param = (char*) PI2PTR(decl, decl->param);
			decl->type  = (char*) PI2PTR(decl, decl->type);
		}

CLEANUP:
	if (NULL != key) {
		freekey (&key);
	}
	return saret;
}

int sa3getcallsby (sa3cg cg, char* func, sa3list* list) {
	int       saret = 0;
	uint16    klen  = 0;
	char*     key   = NULL;
	sa3listit i     = NULL;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);

	CALL_API(saret, getkey ("cr \0", func, &klen, &key));
	CALL_API(saret, getbykey (cg->db, key, klen, list));

	if (NULL != list && NULL != (*list))
		for (i  = sa3firstlstit (*list);
		     i != sa3lastlstit  (*list);
		     i  = sa3nextlstit  (*list, i)) {
			sa3call* call = (sa3call*)sa3resolveit(*list, i);
			call->cd = (char*) PI2PTR(call, call->cd);
			call->cr = (char*) PI2PTR(call, call->cr);
		}

CLEANUP:
	if (NULL != key) {
		freekey (&key);
	}
	return saret;
}

int sa3getcallsof (sa3cg cg, char* func, sa3list* list) {
	int       saret = 0;
	uint16    klen  = 0;
	char*     key   = NULL;
	sa3listit i     = NULL;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);

	CALL_API(saret, getseckey(cg->db, "cd \0", func, &klen, &key));
	CALL_API(saret, getbykey (cg->db, key, klen, list));

	if (NULL != list && NULL != (*list))
		for (i  = sa3firstlstit (*list);
		     i != sa3lastlstit  (*list);
		     i  = sa3nextlstit  (*list, i)) {
			sa3call* call = (sa3call*)sa3resolveit(*list, i);
			call->cd = (char*) PI2PTR(call, call->cd);
			call->cr = (char*) PI2PTR(call, call->cr);
		}

CLEANUP:
	/*if (NULL != key) {
		freekey (&key);
	}*/
	return saret;
}

int sa3getclds    (sa3cg cg, char* func, sa3list* list) {
	int    saret = 0;
	uint16 klen  = 0;
	char*  key   = NULL;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);

	CALL_API (saret, getkey("cld \0", func, &klen, &key));
	CALL_API (saret, getbykey (cg->db, key, klen, list));

	if (NULL == (*list)) {
		// add cld
		CALL_API (saret, addcld (cg->db, func, func) );
		CALL_API (saret, getbykey (cg->db, key, klen, list));
	}

CLEANUP:
	if (NULL != key) {
		freekey (&key);
	}

	return saret;
}

int sa3getprns    (sa3cg cg, char* func, sa3list* list) {
	int    saret = 0;
	uint16 klen  = 0;
	char*  key   = NULL;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);

	CALL_API (saret, getkey ("prn \0", func, &klen, &key));
	CALL_API (saret, getbykey (cg->db, key, klen, list));

	if (NULL == (*list)) {
		// add prn
		CALL_API (saret, addprn (cg->db, func, func) );
		CALL_API (saret, getbykey (cg->db, key, klen, list));
	}

CLEANUP:
	if (NULL != key) {
		freekey (&key);
	}

	return saret;
}
