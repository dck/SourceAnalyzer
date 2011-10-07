#include "sa3internal.h"

#include "sa3internal.c"
#include "sa3common.c"
#include "sa3tools.c"
#include "sa3parsers.c"

struct teststruct {
	char* first;
	uint16 firstlen;
	char* second;
	uint16 secondlen;
};

int sa3test()
{
	int                 saret         = 0;
	sa3cg                cg            = NULL;
	const char*         data2db       = "data\0";
	uint32              datalen2db    = 0;
	char*               datafromdb    = NULL;
	uint32              datalenfromdb = 0;
	char*               key           = NULL;
	struct teststruct*  struc         = NULL;
	DB*                 db            = NULL;

	saret = sa3open (&cg, "test_db");
	EXIT_IF_SAFAIL(saret, "sa3open");
	
	db = cg->db;
	
	{
		const char* pk = "key\0";
		uint32 pklen = 0;
		
		pklen = strlen (pk);
		datalen2db = strlen (data2db) + 1;
		
		saret = put2db (db, (void*) pk, pklen, (void*) data2db, datalen2db);
		EXIT_IF_SAFAIL(saret, "put2db");
	}
	
	{
		const char* pk = "key\0";
		uint32 pklen = 0;
		
		pklen = strlen (pk);
		
		saret = getfirstfromdb (db, (void*) pk, pklen, (void**) &datafromdb, &datalenfromdb);
		EXIT_IF_SAFAIL(saret, "getfirstfromdb");
	}
	
	if (!(datalenfromdb == datalen2db &&
		NULL != data2db && NULL != datafromdb &&
		0 == strncmp (data2db, datafromdb, datalen2db))) {
		fprintf (stderr, "put-get test failed\n");
		if (NULL != data2db)
			fprintf (stderr, "\tdata2db:    %s (%d)\n", data2db, datalen2db);
		else
			fprintf (stderr, "\tdata2db:    NULL (%d)\n", datalen2db);
		if (NULL != datafromdb)
			fprintf (stderr, "\tdatafromdb: %s (%d)\n", datafromdb, datalenfromdb);
		else
			fprintf (stderr, "\tdatafromdb: NULL (%d)\n", datalenfromdb);
		goto cleanup;
	} else {
		fprintf (stderr, "put-get works\n");
		fprintf (stderr, "\tdata2db:    %s (%d)\n", data2db, datalen2db);
		fprintf (stderr, "\tdatafromdb: %s (%d)\n", datafromdb, datalenfromdb);
	}
	
	{
		uint16 keylen = 0;
		const char* prefix = "prefix \0";
		const char* _key = "key\0";
		saret = getkey (prefix, _key, &keylen, &key);
		EXIT_IF_SAFAIL(saret, "getkey");
		if ((strlen(prefix) + strlen(_key) + 1) == keylen &&
			0 == strncmp (key, "prefix key\0", keylen-1)) {
			fprintf (stderr, "key generation is OK\n");
			fprintf (stderr, "\toriginal: %s%s\n\tgetkey:   %s (%d)\n", prefix, _key, key, keylen);
		} else {
			fprintf (stderr, "key generation failed\n");
			fprintf (stderr, "\toriginal: %s%s\n\tgetkey:   %s\n", prefix, _key, key);
			goto cleanup;
		}
	}
	
	{
		uint32 size = 0;
		char* str1 = NULL;
		char* str2 = NULL;
		const char* _str1 = "first line\0";
		const char* _str2 = "second line\0";
		saret = getstruct (&size, (void**)&struc, 2, _str1, _str2);
		EXIT_IF_SAFAIL(saret, "getstruct");
		
		/*PI2PTR(base, shift)*/
		str1 = (char*) PI2PTR(struc, struc->first);
		str2 = (char*) PI2PTR(struc, struc->second);
		if (
			struc->firstlen == (strlen(_str1) + 1) &&
			struc->secondlen == (strlen(_str2) + 1) &&
			0 == strncmp (str1, _str1, struc->firstlen) &&
			0 == strncmp (str2, _str2, struc->secondlen) &&
			size == (struc->firstlen + struc->secondlen + sizeof (struct teststruct))
		) {
			fprintf (stderr, "struct generation works\n");
			fprintf (stderr, "\toriginal:  \"%s\" (%d), \"%s\" (%d)\n", _str1, (strlen(_str1) + 1), _str2, (strlen(_str2) + 1));
			fprintf (stderr, "\tgetstruct: \"%s\" (%d), \"%s\" (%d)\n", str1, struc->firstlen, str2, struc->secondlen);
		} else {
			fprintf (stderr, "struct generation failed (details TBD)\n");
			goto cleanup;
		}				
	}
	
	{
		dep   lst[3];
		dep*  newdep = NULL;
		char* fn1    = "func1\0";
		char* fn2    = "func2\0";
		char* fn3    = "func3\0";
		char* tofnd  = "third\0";
		char* tofnd2 = "second\0";

		lst[0].func = "first\0";
		lst[1].func = "first_?\0";
		lst[2].func = "third\0";

		if ((lst+2) != findlst (tofnd, lst, 3)
		|| NULL != findlst (tofnd2, lst, 3)) {
			fprintf (stderr, "findlst() function failed\n");
			goto cleanup;
		} else {
			fprintf (stderr, "findlst() function works\n");
		}
		
		/* dep* addfnc(sa3cgg cg, char* fn, relation rel) */
		newdep = addfnc (cg, fn1, prn);
		if (NULL == newdep  || newdep != &(cg->prn[0]) || 1 != cg->fnnum
		|| 0 != newdep->fid || 0 != strcmp(fn1, newdep->func)
		|| newdep->func != cg->cld[0].func || newdep->fid != cg->cld[0].fid) {
			fprintf (stderr, "addfnc() function failed\n");
			goto cleanup;
		}
		newdep = addfnc (cg, fn2, cld);
		if (NULL == newdep  || newdep != &(cg->cld[1]) || 2 != cg->fnnum
		|| 1 != newdep->fid || 0 != strcmp(fn2, newdep->func)
		|| newdep->func != cg->cld[1].func || newdep->fid != cg->prn[1].fid) {
			fprintf (stderr, "addfnc() function failed\n");
			goto cleanup;
		}
		newdep = addfnc (cg, fn3, prn);
		fprintf (stderr, "addfnc() function works\n");

		addent2lst (&(cg->cld[0]), &(cg->cld[0]));
		addent2lst (&(cg->cld[1]), &(cg->cld[0]));
		addent2lst (&(cg->cld[2]), &(cg->cld[0]));
		addent2lst (&(cg->cld[2]), &(cg->cld[0]));
		if (cg->cld[0].dpds[0] != &(cg->cld[0])
		||  cg->cld[0].dpds[1] != &(cg->cld[1])
		||  cg->cld[0].dpds[2] != &(cg->cld[2])
		||  cg->cld[0].dpdsnum != 3) {
			fprintf (stderr, "addent2lst() function failed\n");
			goto cleanup;
		} else {
			fprintf (stderr, "addent2lst() function works\n");
		}

		addlst2lst (cg->cld, &(cg->cld[0]), cg->prn, &(cg->prn[2]));
		if (cg->cld[0].dpds[0]->func != cg->prn[2].dpds[0]->func
		||  cg->cld[0].dpds[1]->func != cg->prn[2].dpds[1]->func
		||  cg->cld[0].dpds[2]->func != cg->prn[2].dpds[2]->func
		||  cg->cld[0].dpdsnum != cg->prn[2].dpdsnum) {
			fprintf (stderr, "addlst2lst() function failed\n");
			goto cleanup;
		} else {
			fprintf (stderr, "addlst2lst() function works\n");
		}
		
		fprintf (stderr, "\t%s func, parents deps num %d\n",
				cg->prn[2].func, cg->prn[2].dpdsnum);
		fprintf (stderr, "\t%s func, children deps num %d\n",
				cg->cld[2].func, cg->cld[2].dpdsnum);
		fprintf (stderr, "\t%s func, parents deps num %d\n",
				cg->prn[0].func, cg->prn[0].dpdsnum);
		fprintf (stderr, "\t%s func, children deps num %d\n",
				cg->cld[0].func, cg->cld[0].dpdsnum);
				
		saret = sa3addcall (cg, fn3, fn1);
		EXIT_IF_SAFAIL(saret, "sa3addcall");
		
		if (3 != cg->cld[2].dpdsnum || 3 != cg->prn[0].dpdsnum) {
			int i = 0;
			fprintf (stderr, "sa3addcall() function failed\n");
			
			fprintf (stderr, "Please verify:\n");
			fprintf (stderr, "\t%s func, children deps num %d\n",
				cg->cld[2].func, cg->cld[2].dpdsnum);
			for (i = 0; i < cg->cld[2].dpdsnum; i++) {
				fprintf (stderr, "\t%s func, child # %d: %s\n",
				cg->cld[2].func, i, cg->cld[2].dpds[i]->func);
			}
			fprintf (stderr, "\t%s func, parents deps num %d\n",
				cg->prn[0].func, cg->prn[0].dpdsnum);
			for (i = 0; i < cg->prn[0].dpdsnum; i++) {
				fprintf (stderr, "\t%s func, parent # %d: %s\n",
				cg->prn[0].func, i, cg->prn[0].dpds[i]->func);
			}
			
			goto cleanup;
		} else {
			int i = 0;
			fprintf (stderr, "sa3addcall() seems to work, need additional check.\n");
			fprintf (stderr, "Please verify:\n");
			for (i = 0; i < cg->cld[2].dpdsnum; i++) {
				fprintf (stderr, "\t%s func, child # %d: %s\n",
				cg->cld[2].func, i, cg->cld[2].dpds[i]->func);
			}
			for (i = 0; i < cg->prn[0].dpdsnum; i++) {
				fprintf (stderr, "\t%s func, parent # %d: %s\n",
				cg->prn[0].func, i, cg->prn[0].dpds[i]->func);
			}
			// fprintf();
		}
		
		{
			char*     key      = NULL;
			uint16    klen     = 0;
			sa3list*  list    = NULL;
			uint32    flen     = 0;
			uint32    i        = 0;
			char*     func     = NULL;
			/* char**    strarray = NULL; */

			saret = addrelinfo2db (cg);

			func = "func1\0";
			saret = sa3getprns (cg, func, &list);
			fprintf (stderr, "%s parents:", func);
			for (i = 0; i < list->num; i++) {
				fprintf (stderr, " %s", list->lst[i]);
			}
			fprintf (stderr, "\n");

			func = "func3\0";
			saret = sa3getclds (cg, func, &list);
			fprintf (stderr, "%s parents:", func);
			for (i = 0; i < list->num; i++) {
				fprintf (stderr, " %s", list->lst[i]);
			}
			fprintf (stderr, "\n");

			/*saret = getkey ("prn ", "func1", &klen, &key);
			saret = getfirstfromdb (cg->db, key, klen, (void**) &flist, &flen);

			fprintf (stderr, "%s parents:", "func1");
			strarray = (char**) PI2PTR (flist, flist->lst);
			for (i = 0; i < flist->num; i++) {
				char* str = PI2PTR (flist, strarray[i]);
				fprintf (stderr, " %s", str);
			}

			saret = getkey ("cld ", "func3", &klen, &key);
			saret = getfirstfromdb (cg->db, key, klen, (void**) &flist, &flen);

			fprintf (stderr, "\n%s children:", "func3");
			strarray = (char**) PI2PTR (flist, flist->lst);
			for (i = 0; i < flist->num; i++) {
				char* str = PI2PTR (flist, strarray[i]);
				fprintf (stderr, " %s", str);
			}
			fprintf (stderr, "\n");*/
		}
	}

	saret = sa3close(&cg);
	EXIT_IF_SAFAIL(saret, "sa3close");

cleanup:
	if (0 != saret)
		fprintf (stderr, "SA3 test failed\n");
	else
		fprintf (stderr, "SA3 test passed\n");

	if (NULL != cg)
		sa3close(&cg);

	wrapfree((void**)&key);
	wrapfree((void**)&struc);

	return saret;
}

int main() {
	return sa3test();
}

