#include <db.h>
#include "include/api.h"

static int getfromdb (DB* dbp, void* key, uint16 keylen, u_int32_t flags, sa3list* ppdata) {
	int         saret  = 0;
	DBC*        col    = NULL;
	DBT         keyt, datat;
	sa3list     data   = NULL;
	db_recno_t  dupnum = 0;
	uint32      i      = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (dbp, saret);
	CHK_PTR (key, saret);
	CHK_PTR (ppdata, saret);
	// CHK_PTR (!(*ppdata), saret); // FK: not sure I need it
	CHK_NOTZERO (keylen, saret);
#endif

	/* Zero out the DBTs before using them. */
	memset(&keyt,  0, sizeof(DBT));
	memset(&datat, 0, sizeof(DBT));

	keyt.data = key;
	keyt.size = keylen;

	CALL_API    (saret, wrapmalloc  (&data, sizeof(struct list)));
	CALL_BD_API (saret, dbp->cursor (dbp, NULL, &col, 0));
	CALL_BD_API (saret, col->count  (col, &dupnum, 0));

	if (0 == dupnum) {
		(*ppdata) = NULL;
		goto CLEANUP;
	}

	data->num = (uint32) dupnum;
	CALL_API    (saret, wrapmalloc (&(data->lst), dupnum * sizeof(void*)));

	i = 0;
	CALL_BD_API (saret, col->get (col, &keyt, &datat, flag));
	while (i < data->num) {
		data->lst[i] = datat.data;
		i++;
		CALL_BD_API (saret, col->get (col, &keyt, &datat, DB_NEXT));
	}
	GOTO_CLEANUP_IF (i != data->num, saret, SA3_INTERNAL_ERR);

	data->cur = col;
	(*ppdata) = data;

cleanup:
	if (NULL == (*ppdata)) {
		col->close(col);
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

	ret = dbp->get(pdb, NULL, &keyt, &datat, 0);

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
	CALL_BD_API(saret, cur->put (cur, &keyt, &datat, DB_KEYFIRST));

CLEANUP:
	if (cur != NULL) {
		cur->close(cur);
	}

	return saret;
}

static int putlist2db(DB* pdb, const char* pref, char* func, dep** lst, uint32 lstsize) {
	int        saret    = 0;
	uint32     bsize    = 0;
	int        i        = 0;
	sa3list*   buf      = NULL;
	uint16     keylen   = 0;
	char*      key      = NULL;
	char**     strarray = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (pdb,  saret);
	CHK_STR (pref, saret);
	CHK_STR (func, saret);
	CHK_PTR (lst,  saret);
	CHK_PTR (*lst, saret);
#endif

	// allocate: <struct list>|<char*[lstsize]>
	bsize = sizeof (sa3list) + lstsize * sizeof (char*);
	CALL_API (saret, wrapmalloc (&buf, bsize));

	buf->num = lstsize;
	buf->lst = (char**) sizeof (sa3list);
	strarray = (char**) PI2PTR (buf, buf->lst);
	for (i = 0; i < lstsize; i++) {
		strarray[i] = (char*) bsize;
		bsize  +=  sizeof (char) * lst[i]->flen;
	}

	// reallocate: <struct list>|<char*[lstsize]>|str1|str2|...
	CALL_API (saret, wrapmalloc(&buf, bsize));
	strarray = (char**) PI2PTR (buf, buf->lst);
	for (i = 0; i < lstsize; i++) {
		char* str = PI2PTR (buf, strarray[i]);
		strncpy (str, lst[i]->func, lst[i]->flen);
	}

	CALL_API (saret, getkey (pref, func, &keylen, &key));
	CALL_API (saret, put2db (pdb, key, keylen, (void*) buf, bsize));

CLEANUP:
	if (NULL != key) {
		freekey (&key);
	}

	return saret;
}

static int putrelinfo2db (sa3cg cg) {
	int saret = 0;
	uint32  i = 0;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_CG (cg, saret);
#endif

	for (i = 0; i < cg->fnnum; i++) {
		char*  func    = cg->cld[i].func;
		dep**  lst     = cg->cld[i].dpds;
		uint32 lstsize = cg->cld[i].dpdsnum;
		
		CALL_API (saret, putlist2db(cg->db, "cld ", func, lst, lstsize));
		
		func           = cg->prn[i].func;
		lst            = cg->prn[i].dpds;
		lstsize        = cg->prn[i].dpdsnum;
		
		CALL_API (saret, putlist2db(cg->db, "prn ", func, lst, lstsize));
	}

CLEANUP:
	return saret;
}

int opendb (DB** ppdb, u_int32_t flags) {
	int saret = 0;
	DB* pdb   = NULL;

#ifdef INTERNAL_PARAM_CHECKS
	CHK_PTR (ppdb, saret);
	// CHK_PTR (!(*ppdb), saret); // FK: not sure if this is needed here.
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
	CALL_API (saret, putrelinfo2db (cg));
	cg->db->close(cg->db, 0);
	cg->db = NULL;

CLEANUP:
	return saret;
}
