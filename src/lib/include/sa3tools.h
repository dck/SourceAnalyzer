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
