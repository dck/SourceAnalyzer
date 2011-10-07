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

