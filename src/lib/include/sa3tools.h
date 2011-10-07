#ifndef SA3TOOLS_H
#define SA3TOOLS_H

#include "sa3status.h"
#include "sa3types.h"

/** Opens a call graph. Creates new call garph if specified file is abasent.
 Returns zero on success.
 parameters: 
        cg   - a pointer to sa3cg handle. The handle must be set to NULL first.
	name - file name to a call graph. E.g. "c:\directory\callgraph.cg"
		or "/home/user/cg". */
int sa3open_t     (sa3cg* cg, const char* name);

/** Closes a call graph. Saves the graph to a file. Returns zero on success.
 parameter:
        cg - a pointer to sa3cg handle. Will be set to NULL on success. */
int sa3close_t    (sa3cg* cg);

/** Gets a list of children for the function specified. Returns zero on success.
 Result wouldn't be actual if sa3cg handle wasn't reopened after adding a call.
parameters:
        cg   - sa3cg handle.
	func - null-terminated string with a function name.
	list - a pointer to sa3list which will get a list. A pointer must be
		set to NULL first. */
int sa3getclds    (sa3cg cg, char* func, sa3list* list);

/** Gets a list of parents of the function specified. Interface is similar
 to sa3getclds() */
int sa3getprns    (sa3cg cg, char* func, sa3list* list);

/** Gets a list of declarations of the function specified. Interface is similar
 to sa3getclds() */
int sa3getdecls   (sa3cg cg, char* func, sa3list* list);

/** Gets a list of calls by the function specified. Interface is similar to
 sa3getclds() */
int sa3getcallsby (sa3cg cg, char* func, sa3list* list);

/** Gets a list of calls of the function specified. Interface is similar to
 sa3getclds() */
int sa3getcallsof (sa3cg cg, char* func, sa3list* list);

/** Gets a list of functions. TBD */
int sa3getfuncs   (sa3cg cg, sa3list* list);

/** Free sa3list of functions. TBD */
void sa3freelist  (sa3list* plist);

/** Get firts iterator of specified list. */
sa3listit sa3firstlstit (sa3list lst);

/** Get next iterator. */
sa3listit sa3nextlstit  (sa3list lst, sa3listit it);

/** Get last iterator of specified list. */
sa3listit sa3lastlstit  (sa3list lst);

/** Resolve iterator. */
void*     sa3resolveit  (sa3list lst, sa3listit it);

#endif /* SA3TOOLS_H */
