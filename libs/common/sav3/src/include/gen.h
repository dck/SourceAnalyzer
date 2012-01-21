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

#ifndef GEN_H
#define GEN_H

#define FOREACH_ARG_BEGIN(i, cnt, str) \
	{ \
		va_list vl; \
		va_start(vl, cnt); \
		for (i=0; i < cnt; i++) { \
			str = va_arg(vl, char*);

#define FOREACH_ARG_END \
		} \
		va_end(vl); \
	}

int  getkey     (const char* pref, const char* val, uint16* keylen, char** pkey);
int  gen_cd     (DB *pfdb, const DBT *pkey, const DBT *pdata, DBT *pfkey);
int  gen_cr     (DB *pfdb, const DBT *pkey, const DBT *pdata, DBT *pfkey);
void freekey    (char** pkey);
int  getstruct  (uint32* size, void** struc, uint8 cnt, ...);
void freestruct (void** struc);

#endif // GEN_H
