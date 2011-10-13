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

static int getfromdb (DB* dbp, void* key, uint16 keylen, u_int32_t flag, sa3list* ppdata) {
	int         saret  = 0;
	int         ret    = 0;
	DBC*        cur    = NULL;
	DBT         keyt, datat;
	sa3list     data   = NULL;
	db_recno_t  dupnum = 0;
	uint32      i      = 0;
	u_int32_t   _flag  = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (dbp, saret);
	CHK_PTR (key, saret);
	CHK_PTR (ppdata, saret);
	CHK_NOTZERO (keylen, saret);
#endif

	/* Get a cursor */
	dbp->cursor(dbp, NULL, &cur, 0);

	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	// get size
	dupnum = 0;
	_flag = flag;
	while ((ret = cur->get(cur, &keyt, &datat, _flag)) == 0) {
		if (DB_SET == flag && keylen != keyt.size)
			break;
		if (keylen > keyt.size ||
		    0 != strncmp((const char*)key, (const char*)keyt.data, keylen))
			break;
		dupnum++;
		// fprintf (stderr, "...%d (%s)", dupnum, (char*)datat.data);
		_flag = DB_NEXT;
	}
	// fprintf (stderr, "\n");
	if (ret != DB_NOTFOUND && ret != 0) {
		saret = SA3_DB_ERR;
	}

	if (0 == dupnum) {
		(*ppdata) = NULL;
		goto CLEANUP;
	}

	CALL_API    (saret, wrapmalloc (&data, sizeof(struct list)));
	data->num = (uint32) dupnum;
	CALL_API    (saret, wrapmalloc (&(data->lst), dupnum * sizeof(void*)));

	keyt.data = key;
	keyt.size = keylen;
	// get entries
	_flag = flag;
	for (i = 0;
	     i < data->num && (ret = cur->get(cur, &keyt, &datat, _flag)) == 0;
	     i++) {
		void* tmp = NULL;
		CALL_API (saret, wrapmalloc (&tmp, datat.size));
		memcpy(tmp, datat.data, datat.size);
		data->lst[i] = tmp;
		_flag = DB_NEXT;
	}
	if (ret != DB_NOTFOUND && ret != 0) {
		saret = SA3_DB_ERR;
	}

	data->cur = cur;
	(*ppdata) = data;

CLEANUP:
	if (NULL == (*ppdata) && NULL != cur) {
		cur->close(cur);
	}

	return saret;
}

int getbypref (DB* cg, void* pref, uint16 preflen, sa3list* lst) {
	return getfromdb (cg, pref, preflen, DB_SET_RANGE, lst);
}

int getbykey  (DB* cg, void* key,  uint16 keylen,  sa3list* lst) {
	return getfromdb (cg, key, keylen,  DB_SET,       lst);
}

int getfirstbykey (DB* pdb, void* key, uint16 keylen, void** data, uint32* datalen) {
	int    saret = 0;
	int    ret   = 0;
	DBT keyt, datat;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (pdb,  saret);
	CHK_PTR (key,  saret);
	CHK_PTR (data, saret);
	// CHK_PTR (!(*data), saret); // FK: not sure if I need this.
	CHK_PTR (datalen, saret);
	// CHK_NOTZERO ((*datalen), saret); // FK: same stuff.
#endif

	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	ret = pdb->get(pdb, NULL, &keyt, &datat, 0);

	if (0 != ret) {
		if (DB_NOTFOUND == ret) {
			(*datalen) = 0;
			(*data)    = 0;
		} else {
			// FK: log here ret value.
			saret = SA3_DB_ERR;
			goto CLEANUP;
		}
	} else {
		(*datalen) = datat.size;
		(*data)    = datat.data;
	}

CLEANUP:
	return saret;
}

int put2db (DB* pdb, void* key, uint16 keylen, void* data, uint32 datalen) {
	int saret = 0;
	DBT keyt, datat;
	DBC* cur  = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR     (pdb,     saret);
	CHK_PTR     (key,     saret);
	CHK_PTR     (data,    saret);
	CHK_NOTZERO (datalen, saret);
#endif

	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	datat.data = data;
	datat.size = datalen;
	
	/* Get the cursor */
	CALL_BD_API(saret, pdb->cursor (pdb, NULL, &cur, 0));
	CALL_BD_API(saret, cur->put (cur, &keyt, &datat, /*DB_KEYLAST |*/ DB_NODUPDATA));

CLEANUP:
	if (cur != NULL) {
		cur->close(cur);
	}

	return saret;
}

int ispairexists (DB* db, void* key, uint16 keylen, void* data, uint32 datalen, uint8* flag) {
	int      saret = 0;
	sa3list    lst = NULL;
	sa3listit   it = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR     (db,      saret);
	CHK_PTR     (key,     saret);
	CHK_PTR     (data,    saret);
	CHK_NOTZERO (datalen, saret);
	CHK_PTR     (flag,    saret);
#endif

	*flag = 0;
	CALL_API (saret, getbykey (db, key, keylen, &lst));
	SA3_FORLST (it, lst) {
		void* buf = sa3resolveit (lst, it);
		if (datalen == getbufsize(buf) && 0 == memcmp(buf, data, datalen)) {
			*flag = 1;
			break;
		}
	}

CLEANUP:
	if (lst != NULL) {
		sa3freelist(&lst);
	}

	return saret;
}

int opendb (DB** ppdb, const char* name, u_int32_t flags) {
	int saret = 0;
	DB* pdb   = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (ppdb, saret);
#endif

	CALL_BD_API(saret, db_create (&pdb, NULL, 0));
	CALL_BD_API(saret, pdb->set_flags (pdb, DB_DUPSORT));
	CALL_BD_API(saret, pdb->open (pdb, NULL, name, NULL, DB_BTREE, flags, 0));

	(*ppdb) = pdb;
CLEANUP:
	if (0 != saret) {
		pdb->close(pdb, 0);
	}

	return saret;
}

int closedb (sa3cg cg) {
	int saret = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_CG (cg, saret);
#endif
	// CALL_API (saret, putrelinfo2db (cg));
	cg->db->close(cg->db, 0);
	cg->db = NULL;

CLEANUP:
	return saret;
}
