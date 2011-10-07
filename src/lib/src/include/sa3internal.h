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

#ifndef SA3INTERNAL_H
#define SA3INTERNAL_H

/** Prints error messages. Normally should be used thru log(lvl, msg) macro.
 parameters:
	level - a level of log message, check  enum _loglevel for details.
	file  - a file name of a source file which a message is initiated from.
	line  - a line number from a source file which initiated the message.
	func  - a name of a function which initiated the message.
	msg   - a massage to print out (or not). */
void _log (loglevel level, const char* file, uint32 line, const char* func, const char* msg);

/** Wrapper around realloc(). Works similar, but requires double pointer on
input ans will return SA3 status code. Additionaly fills allocated memory with
zeros. Normally should be used thru wrapmalloc(pptr, len) macro. Returns zero on
success. */
inline int   _wrapmalloc  (void** pptr, size_t len);
/** Wrapper around free(). Works similar, but requires double pointer on input.
Will set a pointer with address to allocated memory to NULL. Normally should be
used thru a wrapfree(pptr) macro. */
inline void  _wrapfree    (void** ptr);

/** Tries to handle Berkeley DB issue. I'm not sure that I should keep it. */
static void  dberrhandle     (int dbret, char* errmsg);
/** Gets a first entity from DB for specified key (pk). Will be replaced with a
 version which returns a collection of all components. Returns zero on success. */
static int   getfirstfromdb  (DB* cg, void* pk, uint32 pklen, void** data, uint32* datalen);
/** Puts data to DB with a specified key (pk). Returns zero on success. */
static int   put2db          (DB* cg, void* pk, uint32 pklen, void* data, uint32 datalen);
/** Generates a key for future processing by put2db() or getfritstfromdb().
 Returns zero on success.
 parameters:
	pref   - a prefix for a key.
	val    - a part which goes after the prefix.
	keylen - a pointer to a variable where key length will be returned
	pkey   - a pointer to pointer which will hold a key. Initial pointer
		must be set to NULL first. */
static int   getkey          (const char* pref, const char* val, uint16* keylen, char** pkey);

/**  Generates a solid blob of a following structure:

     *-------------------------------------------*
     ^                                           |
     |                                           v
 |<shift #1>|<size #1>|......|<sh #N>|<sz #N>|<str #1>|.....|<str #N>
                                 |                              ^
                                 v                              |
                                 *------------------------------*
                                 
 shift #N (sh #N) stands for a shift in bytes from the begining of the blob
	where the string #N is located. 
 size #N (sz #N) stands for a uint16 value which holds the size of string #N.
 str #N is a string #N.
 
 The function is designed to be used with put2db(). The solid blob is needed
 because Berkeley DB requires it. The layout of the blob designed to be easily
 convertable to actual C structures thru the type casing and turning shifts to
 pointers.
 parameters:
	size   - a poiner to a uint32 variable which will hold the size of the blob.
	struct - a pointer to a void* which will hold a blob. The void* variable
		must be set to NULL first.
	cnt    - a number of strings that will be stored in a blob.
	...    - null-terminated strings */
static int  getstruct (uint32* size, void** struc, uint8 cnt, ...);

static int  opencg  (sa3cg* cg, const char* name, u_int32_t flags);
static int  closecg (sa3cg* cg);

#include "sa3tools.h"
#include "sa3parsers.h"
#include "sa3common.h"

#endif /* SA3INTERNAL_H */

