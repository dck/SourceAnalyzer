#ifndef SA3PARSER_H
#define SA3PARSER_H

#include "sa3status.h"
#include "sa3types.h"

/** Opens a call graph. Creates new call garph if specified file is abasent.
 Returns zero on success.
 parameters: 
	cg   - a pointer to sa3cg handle. The handle must be set to NULL first.
	name - file name to a call graph. E.g. "c:\directory\callgraph.cg"
		or "/home/user/cg". */
int sa3open_p  (sa3cg* cg, const char* name);

/** Closes a call graph. Saves the graph to a file. Returns zero on success.
 parameter:
	cg - a pointer to sa3cg handle. Will be set to NULL on success. */
int sa3close_p (sa3cg* cg);

/** Adds a call, to a DB. Updates information on clildren and parents. 
 NOTE that dependency information (parents/children relations) will NOT be
 stored in the DB until sa3close_p() will be executed. Returns zero on success.
 parameters:
	cg - sa3cg handle.
 	cr - null-terminated string which holds caller's function name.
 	cd - null-terminated string which holds a name of called function. */
int sa3addcall (sa3cg cg, char* cr, char* cd);

/** Adds a declaration to DB, interface and return values are similar to
 sa3addcall(). */
int sa3adddecl (sa3cg cg, char* type, char* func, char* param);

/** Merges dependency information (parents/children relations) between two
 call graphs.
 NOTE that dependency information (parents/children relations) will NOT be
 stored in the DB until sa3close_p() will be executed. Returns zero on success.
 parameters:
	rcpt - sa3cg handle. A recipient of merge between itself and src graph.
	src  - sa3cg handle. A graph to merge to rcpt. */
int sa3mergedeps  (sa3cg rcpt, sa3cg src);

/** Merges information about calls between two call graphs. Returns zero on
 success.
 parameters:
	rcpt - sa3cg handle. A recipient of merge between itself and src graph.
	src  - sa3cg handle. A graph to merge to rcpt. */
int sa3mergecalls (sa3cg rcpt, sa3cg src);

/** Merges information about declaration between two call graphs. Returns zero
 on success.
 parameters:
	rcpt - sa3cg handle. A recipient of merge between itself and src graph.
	src  - sa3cg handle. A graph to merge to rcpt. */
int sa3mergedecls (sa3cg rcpt, sa3cg src);

#endif /* SA3PARSER_H */

