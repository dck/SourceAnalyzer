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

int get_ent_pk (database_t* pdb, void* key, uint16 keylen, void** ppdata, uint32* pdatalen) {
	int saret = 0;
	DBT keyt, datat;
	void* tmp = NULL;

	CHK_I_PTR (pdb, saret);
	CHK_I_PTR (key, saret);
	CHK_I_PTR (ppdata, saret);
	CHK_I_PTR (pdatalen, saret);
	CHK_I_NOTZERO (keylen, saret);
	
	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	CALL_BD_API(saret, pdb->pdb->get(pdb->pdb, NULL, &keyt, &datat, 0));
	CALL_API (saret, wrapmalloc (&tmp, datat.size));
	memcpy(tmp, datat.data, datat.size);
	*ppdata   = tmp;
	*pdatalen = (uint32)datat.size;

CLEANUP:

	return saret;
}

// FK: TODO: refactor, function is too big
static int get_ent_internal (database_t* pdb, void* key, uint16 keylen, sa3list* pdata, uint8 FK_ID, u_int32_t flag) {
	int         saret  = 0;
	int         ret    = 0;
	DB*         dbp    = NULL;
	DBC*        cur    = NULL;
	DBT         keyt, datat;
	sa3list     data   = NULL;
	db_recno_t  dupnum = 0;
	uint32      i      = 0;
	u_int32_t   _flag  = 0;

	CHK_I_PTR (pdb, saret);
	CHK_I_PTR (pdata, saret);

	if        (CR  == FK_ID) {
		dbp = pdb->pdb_cr;
	} else if (CD  == FK_ID) {
		dbp = pdb->pdb_cd;
	} else if (DL  == FK_ID) {
		dbp = pdb->pdb_dl;
	} else if (CHD == FK_ID) {
		dbp = pdb->pdb_chd;
	} else if (PRN == FK_ID) {
		dbp = pdb->pdb_prn;
	} else {
		GOTO_CLEANUP(saret, SA3_INTERNAL_ERR);
	}

	/* Get a cursor */
	CALL_BD_API(saret, dbp->cursor(dbp, NULL, &cur, 0));

	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	// get size
	dupnum = 0;
	_flag = flag;
	while ((ret = cur->get(cur, &keyt, &datat, _flag)) == 0) {
		if (DB_SET == flag && (keylen != keyt.size || 0 != memcmp(keyt.data, key, keylen)))
			break;
		dupnum++;
		_flag = DB_NEXT;
	}
	// fprintf (stderr, "\n");
	if (ret != DB_NOTFOUND && ret != 0) {
		GOTO_CLEANUP(saret, SA3_DB_ERR);
	}

	if (0 == dupnum) {
		(*pdata) = NULL;
		goto CLEANUP;
	}

	CALL_API    (saret, wrapmalloc (&data, sizeof(struct list)));
	data->num = (uint32) dupnum;
	CALL_API    (saret, wrapmalloc (&(data->lst), dupnum * sizeof(void*)));

	keyt.data = key;
	keyt.size = keylen;
	// get entries
	_flag = flag;
	for (i = 0; i < data->num && (ret = cur->get(cur, &keyt, &datat, _flag)) == 0; i++) {
		void* tmp = NULL;
		CALL_API (saret, wrapmalloc (&tmp, datat.size));
		memcpy(tmp, datat.data, datat.size);
		data->lst[i] = tmp;
		_flag = DB_NEXT;
	}
	if (ret != DB_NOTFOUND && ret != 0) {
		GOTO_CLEANUP (saret, SA3_DB_ERR);
	}

	data->cur = cur;
	(*pdata) = data;

CLEANUP:

	if (NULL == (*pdata) && NULL != cur) {
		cur->close(cur);
	}
	if (0 != saret) {
		if (NULL != data) {
			if (NULL != data->lst) {
				for (i = 0; i < dupnum; i++) {
					if (NULL != data->lst[i]) {
						wrapfree(&(data->lst[i]));
					}
				}
				wrapfree(&(data->lst));
			}
			wrapfree(&data);
		}
		// (*pdata) = 0; // FK: TODO: think twice if I need this.
	}

	return saret;
}

int get_ent (database_t* pdb, void* key, uint16 keylen, sa3list* pdata, uint8 ID) {
	return get_ent_internal (pdb, key, keylen, pdata, ID, DB_SET);
}

int get_all_ent  (database_t*  pdb, sa3list* pdata, uint8 ID) {
	return get_ent_internal (pdb, NULL, 0, pdata, ID, DB_FIRST);
}

int put_ent (database_t* pdb, void* key, uint16 keylen, void* data, uint32 datalen, uint8 ID) {
	int saret = 0;
	DB* db = NULL;
	DBT keyt, datat;
	DBC* cur  = NULL;
	u_int32_t flags;
	uint32 tmpkey = 0;

	CHK_I_PTR     (pdb,     saret);
	CHK_I_PTR     (data,    saret);
	CHK_I_NOTZERO (datalen, saret);

	flags = 0;
	if        (CR  == ID || CD  == ID) {
		db = pdb->pdb;
	} else if (DL  == ID) {
		db = pdb->pdb_dl;
	} else if (CHD == ID) {
		db = pdb->pdb_chd;
	} else if (PRN == ID) {
		db = pdb->pdb_prn;
	} else {
		GOTO_CLEANUP(saret, SA3_INTERNAL_ERR);
	}

	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	if (NULL != key && 0 != keylen) {
		keyt.data = key;
		keyt.size = keylen;
	} else if (NULL == key && 0 == keylen) {
		tmpkey = pdb->db_cnt;
		keyt.data = (void*)&tmpkey;
		keyt.size = sizeof(uint32);
		pdb->db_cnt++;
	} else {
		GOTO_CLEANUP(saret, SA3_INTERNAL_ERR);
	}

	datat.data = data;
	datat.size = datalen;
	
	if (DL  == ID || CHD == ID || PRN == ID) {
		/* http://docs.oracle.com/cd/E17076_02/html/programmer_reference/general_am_conf.html
		  It is an error to attempt to store identical duplicate data items
		  when duplicates are being stored in a sorted order. Any such attempt
		  results in the error message "Duplicate data items are not supported
		  with sorted data" with a DB_KEYEXIST return code.

		  Note that you can suppress the error message "Duplicate data items
		  are not supported with sorted data" by using the DB_NODUPDATA flag. */
		flags = DB_NODUPDATA;
	}

	CALL_BD_API(saret, db->put  (db, NULL, &keyt, &datat, flags));

CLEANUP:
	if (cur != NULL) {
		cur->close(cur);
	}

	return saret;
}

int is_ent_exist (database_t* pdb, void* key, uint16 keylen, void* data, uint32 datalen, uint8 ID, uint8* flag) {
	int      saret = 0;
	sa3list    lst = NULL;
	sa3listit   it = NULL;

	CHK_I_PTR     (pdb,      saret);
	CHK_I_PTR     (key,     saret);
	CHK_I_PTR     (data,    saret);
	CHK_I_NOTZERO (datalen, saret);
	CHK_I_PTR     (flag,    saret);

	*flag = 0;
	CALL_API (saret, get_ent (pdb, key, keylen, &lst, ID));
	SA3_FORLST (it, lst) {
		void* buf = NULL;
		buf = (void*) sa3resolveit (lst, it);
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

static int open_db_internal (database_t** ppdb, const char* name, u_int32_t flags, u_int32_t first_flags) {
	int     saret   = 0;
	DB*     pdb     = NULL;
	DB*     pdb_cr  = NULL;
	DB*     pdb_cd  = NULL;
	DB*     pdb_dl  = NULL;
	DB*     pdb_chd = NULL;
	DB*     pdb_prn = NULL;
	DB_ENV* pdb_en  = NULL;
	database_t* phandle = NULL;

	CHK_I_PTR (ppdb,  saret);
	CHK_I_STR (name,  saret);

	CALL_BD_API(saret, db_env_create (&pdb_en, 0));
	CALL_BD_API(saret, pdb_en->open  (pdb_en, NULL, DB_CREATE | DB_INIT_MPOOL | DB_THREAD, 0));
	CALL_BD_API(saret, db_create (&pdb,     pdb_en, 0));
	CALL_BD_API(saret, db_create (&pdb_cr,  pdb_en, 0));
	CALL_BD_API(saret, db_create (&pdb_cd,  pdb_en, 0));
	CALL_BD_API(saret, db_create (&pdb_dl,  pdb_en, 0));
	CALL_BD_API(saret, db_create (&pdb_chd, pdb_en, 0));
	CALL_BD_API(saret, db_create (&pdb_prn, pdb_en, 0));
	// CALL_BD_API(saret, pdb->set_flags     (pdb,     DB_DUPSORT));
	CALL_BD_API(saret, pdb_cr->set_flags  (pdb_cr,  DB_DUPSORT));
	CALL_BD_API(saret, pdb_cd->set_flags  (pdb_cd,  DB_DUPSORT));
	CALL_BD_API(saret, pdb_dl->set_flags  (pdb_dl,  DB_DUPSORT));
	CALL_BD_API(saret, pdb_chd->set_flags (pdb_chd, DB_DUPSORT));
	CALL_BD_API(saret, pdb_prn->set_flags (pdb_prn, DB_DUPSORT));
	flags = flags | DB_THREAD;
	CALL_BD_API(saret, pdb->open      (pdb,     NULL, name, "id_keys",  DB_BTREE, flags | first_flags, 0));
	CALL_BD_API(saret, pdb_cr->open   (pdb_cr,  NULL, name, "cr_keys",  DB_BTREE, flags, 0));
	CALL_BD_API(saret, pdb_cd->open   (pdb_cd,  NULL, name, "cd_keys",  DB_BTREE, flags, 0));
	CALL_BD_API(saret, pdb_dl->open   (pdb_dl,  NULL, name, "dl_keys",  DB_BTREE, flags, 0));
	CALL_BD_API(saret, pdb_chd->open  (pdb_chd, NULL, name, "chd_keys", DB_BTREE, flags, 0));
	CALL_BD_API(saret, pdb_prn->open  (pdb_prn, NULL, name, "prn_keys", DB_BTREE, flags, 0));
	CALL_BD_API(saret, pdb->associate (pdb, NULL, pdb_cr, gen_cr, 0));
	CALL_BD_API(saret, pdb->associate (pdb, NULL, pdb_cd, gen_cd, 0));
	
	CALL_API(saret, wrapmalloc(&phandle, sizeof(database_t)));
	
	phandle->pdb = pdb;
	phandle->pdb_cr  = pdb_cr;
	phandle->pdb_cd  = pdb_cd;
	phandle->pdb_en  = pdb_en;
	phandle->pdb_dl  = pdb_dl;
	phandle->pdb_chd = pdb_chd;
	phandle->pdb_prn = pdb_prn;
	phandle->db_cnt  = 0;
	*ppdb = phandle;

CLEANUP:
	if (0 != saret) {
		if (NULL != pdb_dl)
			pdb_dl->close (pdb_dl, 0);
		if (NULL != pdb_chd)
			pdb_chd->close (pdb_chd, 0);
		if (NULL != pdb_cr)
			pdb_cr->close (pdb_cr, 0);
		if (NULL != pdb_cd)
			pdb_cd->close (pdb_cd, 0);
		if (NULL != pdb)
			pdb->close    (pdb, 0);
		if (NULL != pdb_en)
			pdb_en->close (pdb_en, 0);
		if (NULL != phandle)
			wrapfree(&phandle);
	}

	return saret;
}

int create_db (database_t** ppdb, const char* name) {
	return open_db_internal (ppdb, name, DB_CREATE, DB_TRUNCATE);
}

int open_db (database_t** ppdb, const char* name) {
	return open_db_internal (ppdb, name, 0, 0);
}

int close_db (database_t** ppdb) {

#define CLOSE_DB(pdb) { if (NULL != pdb) pdb->close (pdb, 0); }

	int saret = 0;
	database_t* pdb = NULL;

	CHK_I_PTR (ppdb, saret);

	pdb = *ppdb;
	if (NULL != pdb) {
		CLOSE_DB(pdb->pdb_cr);
		CLOSE_DB(pdb->pdb_cd);
		CLOSE_DB(pdb->pdb);
		CLOSE_DB(pdb->pdb_dl);
		CLOSE_DB(pdb->pdb_chd);
		CLOSE_DB(pdb->pdb_prn);
		CLOSE_DB(pdb->pdb_en);
		wrapfree(ppdb);
	}

#undef CLOSE_DB

CLEANUP:

	return saret;
}
