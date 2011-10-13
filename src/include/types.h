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

#ifndef TYPES_H
#define TYPES_H

typedef  uint8_t   uint8;
typedef  uint16_t  uint16;
typedef  uint32_t  uint32;

struct model {
	char*   val;
	uint16  len;
};

struct list {
	uint32  num;
	char**  lst;
	DBC*    cur;
};
typedef struct list* sa3list;
typedef void** sa3listit;

/*
struct _dep {
	char*           func;
	uint16          flen;
	uint32          fid;
	struct _dep**   dpds;
	uint32          dpdsnum;
	uint32          bfsize;
};
typedef struct _dep dep;
*/

struct cg {
	DB*     db;
/*	dep*    cld;
	dep*    prn;
	uint32  fnnum;
	uint32  bfsize;*/
};
typedef struct cg* sa3cg;

struct _call {
	char* cr;
	uint16 crlen;
	char* cd;
	uint16 cdlen;
};
typedef struct _call sa3call;

struct _decl {
	char* type;
	uint16 typelen;
	char* func;
	uint16 funclen;
	char* param;
	uint16 paramlen;
};
typedef struct _decl sa3decl;

enum _rel {
	prn = 1,
	cld
};
typedef enum _rel rel;

enum _loglevel {
	error = 0,
	warning,
	info,
};
typedef enum _loglevel loglevel;

#endif // TYPES_H
