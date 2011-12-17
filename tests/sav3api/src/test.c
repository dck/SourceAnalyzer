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

#include "api.h"

#include "gen.c"
#include "malloc.c"
#include "tools.c"
#include "cg.c"
#include "parsers.c"
#include "wrapdb.c"

struct teststruct {
	char* first;
	uint16 firstlen;
	char* second;
	uint16 secondlen;
};

int sa3test()
{
	int                 saret         = 0;
	sa3cg               cg            = NULL;
	const char*         data2db       = "data1\0";
	const char*         data2db2      = "data2\0";
	const char*         data2db3      = "data3\0";
	uint32              datalen2db    = 0;
	char*               datafromdb    = NULL;
	uint32              datalenfromdb = 0;
	char*               key           = NULL;
	struct teststruct*  struc         = NULL;
	DB*                 db            = NULL;

	// wrapmalloc/free test
	{
		void* ptr = NULL;
		void* bp  = NULL;
		int   i   = 0;
		CALL_API(saret, wrapmalloc(&ptr, 5));

		for(i = 0; i < 5; i++) {
			uint8* n = PI2PTR(ptr, i);
			*n = 0x1;
		}

		bp = BUF2SIZE(ptr);
		fprintf(stderr, "ptr:  %x\nbase: %x\nMemory (base): ", (uint32)ptr, (uint32)bp);
		for(i = 0; i < 10; i++)
			fprintf(stderr, "0x%x ", *PI2PTR(bp, i));
		fprintf(stderr, "\n");
		CALL_API(saret, wrapmalloc(&ptr, 10));
		bp = BUF2SIZE(ptr);
		fprintf(stderr, "ptr:  %x\nbase: %x\nMemory (base): ", (uint32)ptr, (uint32)bp);
		for(i = 0; i < 15; i++)
			fprintf(stderr, "0x%x ", *PI2PTR(bp, i));
		fprintf(stderr, "\n");
		wrapfree(&ptr);
		fprintf(stderr, "ptr:  %x\n", (uint32)ptr);
	}

	// sa3open_p test
	{	
		CALL_API(saret, sa3open_p (&cg, "test_db"));
		db = cg->db;
	}
	
	// put2db test
	{
		const char* pk = "key\0";
		uint32 pklen = 0;

		pklen = strlen (pk);
		datalen2db = strlen (data2db) + 1;

		CALL_API(saret, put2db (db, (void*) pk, pklen, (void*) data2db, datalen2db));
	}
	
	// put2db test
	{
		const char* pk = "kehh\0";
		uint32 pklen = 0;

		pklen = strlen (pk);
		datalen2db = strlen (data2db2) + 1;

		CALL_API(saret, put2db (db, (void*) pk, pklen, (void*) data2db2, datalen2db));
	}

	// put2db test
	{
		const char* pk = "tehh\0";
		uint32 pklen = 0;

		pklen = strlen (pk);
		datalen2db = strlen (data2db3) + 1;

		CALL_API(saret, put2db (db, (void*) pk, pklen, (void*) data2db3, datalen2db));
	}

	// getfirstbykey test
	{
		const char* pk = "key\0";
		uint32 pklen = 0;
		
		pklen = strlen (pk);
		
		CALL_API(saret, getfirstbykey (db, (void*) pk, pklen, (void**) &datafromdb, &datalenfromdb));
	
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
			goto CLEANUP;
		} else {
			fprintf (stderr, "put-get works\n");
			fprintf (stderr, "\tdata2db:    %s (%d)\n", data2db, datalen2db);
			fprintf (stderr, "\tdatafromdb: %s (%d)\n", datafromdb, datalenfromdb);
		}
	}
	
	// BDB experimenting
	{
		DBC *cursorp;
		DBT key, data;
		int ret;
		u_int32_t flag = 0;
		char*   keystr = "key\0";
		size_t  keylen = 0;

		/* Database open omitted for clarity */

		/* Get a cursor */
		db->cursor(db, NULL, &cursorp, 0);

		/* Initialize our DBTs. */
		memset(&key,  0, sizeof(DBT));
		memset(&data, 0, sizeof(DBT));

		keylen = strlen(keystr);

		/* Get the keys */
		key.data = keystr;
		key.size = keylen; // + 1;

		/* Iterate over the database, retrieving each record in turn. */
		flag = DB_SET_RANGE;
		while ((ret = cursorp->get(cursorp, &key, &data, flag)) == 0) {
			if (keylen > key.size || 0 != strncmp(keystr, key.data, keylen))
				break;
			fprintf (stderr, "\t\t%s\n", (char*)data.data);
			flag = DB_NEXT;
		}
		if (ret != DB_NOTFOUND && ret != 0) {
			fprintf (stderr, "\t\terror\n");
		} else {
			fprintf (stderr, "\t\tOK\n");
		}
	}
	//exit(2);

	// getbypref, sa3freelist (cursor) test
	{
		sa3list  lst = NULL;
		uint32     i = 0;
		char*   pref = "ke\0";

		// FK: check out during testing whether length must be
		// strlen(pref) or strlen(pref)+1 (with null-term symbol).
		CALL_API (saret, getbypref (db, pref, strlen(pref), &lst));
		fprintf (stderr, "getbypref found %d entries which starts with %s (please verify):\n", lst->num, pref);
		for (i = 0; i < lst->num; i++) {
			fprintf (stderr, "\t#%d %s", i, lst->lst[i]);
		}
		fprintf (stderr, "\n");
		sa3freelist (&lst);
	}
	//exit(2);

	// getkey test
	{
		uint16 keylen = 0;
		const char* prefix = "prefix \0";
		const char* _key = "key\0";
		CALL_API(saret, getkey (prefix, _key, &keylen, &key));
		if ((strlen(prefix) + strlen(_key) + 1) == keylen &&
			0 == strncmp (key, "prefix key\0", keylen-1)) {
			fprintf (stderr, "key generation is OK\n");
			fprintf (stderr, "\toriginal: %s%s\n\tgetkey:   %s (%d)\n", prefix, _key, key, keylen);
		} else {
			fprintf (stderr, "key generation failed\n");
			fprintf (stderr, "\toriginal: %s%s\n\tgetkey:   %s\n", prefix, _key, key);
			goto CLEANUP;
		}
	}

	// getstruct test
	{
		uint32 size = 0;
		char* str1 = NULL;
		char* str2 = NULL;
		const char* _str1 = "first line\0";
		const char* _str2 = "second line\0";
		CALL_API(saret, getstruct (&size, (void**)&struc, 2, _str1, _str2));

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
			goto CLEANUP;
		}				
	}
	
	{
		/*dep   lst[3];
		dep*  newdep   = NULL;*/
		char* fn1type  = "void\0";
		char* fn1param = "void* ptr, int n\0";
		char* fn2type  = "int\0";
		char* fn2param = "int* pnum, char* str\0";
		char* fn1      = "func1\0";
		char* fn2      = "func2\0";
		char* fn3      = "func3\0";
		char* tofnd    = "third\0";
		char* tofnd2   = "second\0";

		// findlst test
		/*{
			dep* d[2] = {NULL, NULL};
			lst[0].func = "first\0";
			lst[1].func = "first_?\0";
			lst[2].func = "third\0";

			CALL_API(saret, findlst (tofnd,  lst, 3, &(d[0])));
			CALL_API(saret, findlst (tofnd2, lst, 3, &(d[1])));

			if ((lst+2) != d[0]
			|| NULL     != d[1]) {
				fprintf (stderr, "findlst() function failed\n");
				goto CLEANUP;
			} else {
				fprintf (stderr, "findlst() function works\n");
			}
		}*/

		// addfnc test
		/*{
			CALL_API(saret, addfnc (cg, fn1, prn, &newdep));
			if (NULL == newdep  || newdep != &(cg->prn[0]) || 1 != cg->fnnum
			|| 0 != newdep->fid || 0 != strcmp(fn1, newdep->func)
			|| newdep->func != cg->cld[0].func || newdep->fid != cg->cld[0].fid) {
				fprintf (stderr, "addfnc() function failed\n");
				goto CLEANUP;
			}
			CALL_API(saret, addfnc (cg, fn2, cld, &newdep));
			if (NULL == newdep  || newdep != &(cg->cld[1]) || 2 != cg->fnnum
			|| 1 != newdep->fid || 0 != strcmp(fn2, newdep->func)
			|| newdep->func != cg->cld[1].func || newdep->fid != cg->prn[1].fid) {
				fprintf (stderr, "addfnc() function failed\n");
				goto CLEANUP;
			}
			CALL_API(saret, addfnc (cg, fn3, prn, &newdep));
			// TODO: addfnc check
			fprintf (stderr, "addfnc() function works\n");
		}*/

		// addent2lst test
		/*{
			CALL_API(saret, addent2lst (&(cg->cld[0]), &(cg->cld[0])));
			CALL_API(saret, addent2lst (&(cg->cld[1]), &(cg->cld[0])));
			CALL_API(saret, addent2lst (&(cg->cld[2]), &(cg->cld[0])));
			CALL_API(saret, addent2lst (&(cg->cld[2]), &(cg->cld[0])));
			if (cg->cld[0].dpds[0] != &(cg->cld[0])
			||  cg->cld[0].dpds[1] != &(cg->cld[1])
			||  cg->cld[0].dpds[2] != &(cg->cld[2])
			||  cg->cld[0].dpdsnum != 3) {
				fprintf (stderr, "addent2lst() function failed\n");
				goto CLEANUP;
			} else {
				fprintf (stderr, "addent2lst() function works\n");
			}

			CALL_API(saret, addlst2lst (cg->cld, &(cg->cld[0]), cg->prn, &(cg->prn[2])));
			if (cg->cld[0].dpds[0]->func != cg->prn[2].dpds[0]->func
			||  cg->cld[0].dpds[1]->func != cg->prn[2].dpds[1]->func
			||  cg->cld[0].dpds[2]->func != cg->prn[2].dpds[2]->func
			||  cg->cld[0].dpdsnum != cg->prn[2].dpdsnum) {
				fprintf (stderr, "addlst2lst() function failed\n");
				goto CLEANUP;
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
		}*/

		// sa3addcall test
		/*{
			CALL_API(saret, sa3addcall (cg, fn3, fn1));
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

				goto CLEANUP;
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
		}*/

		// sa3addcall, sa3getcallsby test
		{
			sa3list list = NULL;
			sa3listit  i = NULL;
			char* fn4 = "func4\0";
			char* fn5 = "func5\0";
			char* fn6 = "func6\0";
			char* fn7 = "func7\0";
			CALL_API(saret, sa3addcall    (cg, fn4, fn5));
			CALL_API(saret, sa3addcall    (cg, fn4, fn6));
			CALL_API(saret, sa3addcall    (cg, fn4, fn7));
			CALL_API(saret, sa3getcallsby (cg, fn4, &list));
			if (NULL != list) {
				fprintf (stderr, "sa3getcallsby, found %d calls with %s as caller\n", list->num, fn4);
				for (i  = sa3firstlstit (list);
				     i != sa3lastlstit  (list);
				     i  = sa3nextlstit  (list, i)) {
					sa3call* call = (sa3call*)sa3resolveit(list, i);
					fprintf (stderr, "\t%s -> %s\n", call->cr, call->cd);
				}
			} else {
				fprintf (stderr, "sa3getcallsby, found nothing for %s\n", fn4);
			}
		}

		// sa3adddecl, sa3getdecls test
		{
			sa3list list = NULL;
			sa3listit  i = NULL;
			CALL_API(saret, sa3adddecl  (cg, fn1type, fn1, fn1param));
			CALL_API(saret, sa3adddecl  (cg, fn2type, fn1, fn2param));
			CALL_API(saret, sa3getdecls (cg, fn1, &list));
			fprintf (stderr, "sa3adddecl, found %d declarations that match %s\n", list->num, fn1);
			for (i  = sa3firstlstit (list);
			     i != sa3lastlstit  (list);
			     i  = sa3nextlstit  (list, i)) {
				sa3decl* decl = (sa3decl*)sa3resolveit(list, i);
				fprintf (stderr, "\t%s %s (%s)\n", decl->type, decl->func, decl->param);
			}
		}

		// sa3close_p test
		{
			CALL_API(saret, sa3close_p(&cg));
		}

		// sa3open_t test
		{
			CALL_API(saret, sa3open_t (&cg, "test_db"));
		}
		
		{
			char*     key      = NULL;
			uint16    klen     = 0;
			sa3list   list     = NULL;
			uint32    flen     = 0;
			uint32    i        = 0;
			char*     func     = NULL;

			/* sa3getprns, sa3freelist (blob) test
			{
				sa3listit it = NULL;
				func = "func1\0";
				CALL_API(saret, sa3getprns (cg, func, &list));
				fprintf (stderr, "%s parents:\t", func);
				for (it  = sa3firstlstit (list);
				     it != sa3lastlstit  (list);
				     it  = sa3nextlstit  (list, it)) {
					fprintf (stderr, " %s (0x%x)",
						 (char*) sa3resolveit(list, it),
						 (uint32)sa3resolveit(list, it));
				}
				fprintf (stderr, "\n");
				sa3freelist (&list);

			}*/

			/* sa3getclds, sa3freelist (blob) test
			{
				sa3listit it = NULL;
				func = "func3\0";
				CALL_API(saret, sa3getclds (cg, func, &list));
				fprintf (stderr, "%s children:", func);
				for (it  = sa3firstlstit (list);
				     it != sa3lastlstit  (list);
				     it  = sa3nextlstit  (list, it)) {
					fprintf (stderr, " %s (0x%x)",
						 (char*) sa3resolveit(list, it),
						 (uint32)sa3resolveit(list, it));
				}
				fprintf (stderr, "\n");
				sa3freelist (&list);
			}*/

		}

		// sa3mergedeps test
		{
			sa3cg cg2link[2];
			sa3cg cg4res = NULL;
			char* f[] = {"1f\0", "2f\0", "3f\0", "4f\0"};

			fprintf (stderr, "Linkage (sa3mergedeps) testing\n");

			fprintf (stderr, "\tCreating the first call graph for linkage (f1->f2->f3)... ");
			{
				CALL_API(saret, sa3open_p  (&cg2link[0], "link_test_db_1"));
				CALL_API(saret, sa3adddecl (cg2link[0], "void", f[0], "int"));
				CALL_API(saret, sa3adddecl (cg2link[0], "void\0", f[1], "uint\0"));
				CALL_API(saret, sa3adddecl (cg2link[0], "void", f[2], "size_t"));
				CALL_API(saret, sa3addcall (cg2link[0], f[2], f[1]));
				CALL_API(saret, sa3addcall (cg2link[0], f[0], f[1]));
				CALL_API(saret, sa3addcall (cg2link[0], f[1], f[2]));
				CALL_API(saret, sa3close_p (&(cg2link[0])));
				fprintf (stderr, "done!\n");
			}

			fprintf (stderr, "\tCreating the second call graph for linkage (f4->f1; f4->f2)... ");
			{
				CALL_API(saret, sa3open_p  (&cg2link[1], "link_test_db_2"));
				CALL_API(saret, sa3adddecl (cg2link[1], "void\0", f[1], "uint\0"));
				CALL_API(saret, sa3adddecl (cg2link[1], "int\0", f[3], "\0"));
				CALL_API(saret, sa3addcall (cg2link[1], f[3], f[0]));
				CALL_API(saret, sa3addcall (cg2link[1], f[3], f[1]));
				CALL_API(saret, sa3close_p (&(cg2link[1])));
				fprintf (stderr, "done!\n");
			}

			fprintf (stderr, "\tLinkage... ");
			{
				CALL_API(saret, sa3open_p  (&cg4res,     "link_test_db_3"));
				CALL_API(saret, sa3open_t  (&cg2link[0], "link_test_db_1"));
				CALL_API(saret, sa3open_t  (&cg2link[1], "link_test_db_2"));

				CALL_API(saret, sa3mergecalls (cg4res, cg2link[0]));
				CALL_API(saret, sa3mergecalls (cg4res, cg2link[1]));

				CALL_API(saret, sa3mergedecls (cg4res, cg2link[0]));
				CALL_API(saret, sa3mergedecls (cg4res, cg2link[1]));

				CALL_API(saret, sa3close_t (&(cg2link[0])));
				CALL_API(saret, sa3close_t (&(cg2link[1])));
				CALL_API(saret, sa3close_p (&(cg4res)));
				fprintf (stderr, "done!\n");
			}

			fprintf (stderr, "\tPrinting results (must be f4 cld: f1, f2, f3, f2 prn: f1, f4, etc.)\n");
			{
				int i = 0;
				CALL_API(saret, sa3open_t  (&cg4res,     "link_test_db_3"));
				CALL_API(saret, sa3open_t  (&cg2link[0], "link_test_db_1"));
				CALL_API(saret, sa3open_t  (&cg2link[1], "link_test_db_2"));

				fprintf (stderr, "\t\tfirst call graph\n");
				for (i = 0; i < 3; i++) {
					sa3listit  it = NULL;
					sa3list   lst = NULL;
					fprintf (stderr, "\t\t%s relations:\n", f[i]);
					CALL_API(saret, sa3getprns(cg2link[0], f[i], &lst));
					fprintf (stderr, "\t\t\tparents: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					CALL_API(saret, sa3getclds(cg2link[0], f[i], &lst));
					fprintf (stderr, "\n\t\t\tchildren: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					fprintf (stderr, "\n");
				}

				fprintf (stderr, "\t\tsecond call graph\n");
				for (i = 0; i < 4; i++) {
					sa3listit  it = NULL;
					sa3list   lst = NULL;
					if (2 == i)
						continue;
					fprintf (stderr, "\t\t%s relations:\n", f[i]);
					CALL_API(saret, sa3getprns(cg2link[1], f[i], &lst));
					fprintf (stderr, "\t\t\tparents: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					CALL_API(saret, sa3getclds(cg2link[1], f[i], &lst));
					fprintf (stderr, "\n\t\t\tchildren: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					fprintf (stderr, "\n");
				}

				fprintf (stderr, "\t\tfinal call graph\n");
				for (i = 0; i < 4; i++) {
					sa3listit  it = NULL;
					sa3list   lst = NULL;
					fprintf (stderr, "\t\t%s relations:\n", f[i]);
					CALL_API(saret, sa3getprns(cg4res, f[i], &lst));
					fprintf (stderr, "\t\t\tparents: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					CALL_API(saret, sa3getclds(cg4res, f[i], &lst));
					fprintf (stderr, "\n\t\t\tchildren: ");
					SA3_FORLST (it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit(lst, it));
					}
					sa3freelist(&lst);
					fprintf (stderr, "\n");
				}

				fprintf (stderr, "\t\tfunc list obtain: ");
				{
					sa3list   lst = NULL;
					sa3listit it  = NULL;

					CALL_API(saret, sa3getfuncs (cg4res, &lst));
					SA3_FORLST(it, lst) {
						fprintf (stderr, "%s ", (char*)sa3resolveit (lst, it));
					}
					fprintf (stderr, "\n");
				}

				CALL_API(saret, sa3close_t (&cg4res));
				fprintf (stderr, "\tDone!\n");
			}


		}

		// sa3close_t test
		{
			CALL_API(saret, sa3close_t(&cg));
		}
	}

CLEANUP:
	{
		if (0 != saret)
			fprintf (stderr, "\nSA3 test failed\n");
		else
			fprintf (stderr, "\nSA3 test passed\n");

		if (NULL != cg)
			sa3close_p(&cg);

		if (NULL != key)
			wrapfree((void**)&key);
		if (NULL != struc)
			wrapfree((void**)&struc);

		return saret;
	}
}

int main() {
	return sa3test();
}

