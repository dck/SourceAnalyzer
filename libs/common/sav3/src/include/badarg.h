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

#ifndef BADARG_H
#define BADARG_H

/* FK: why 300? This is SPARTA? */
#define MAX_STR_LEN  (300)

#define CHK_PTR(ptr, saret)      { GOTO_CLEANUP_IF (NULL == (ptr),    saret, SA3_NULLPTR_ERR); }
#define CHK_CG( cg,  saret)      { GOTO_CLEANUP_IF (NULL == (cg),     saret, SA3_NULLPTR_ERR); \
				   GOTO_CLEANUP_IF (NULL == (cg)->db, saret, SA3_NULLPTR_ERR); }
#define CHK_STR(str, saret)      { void* nullch = NULL;                                        \
				   GOTO_CLEANUP_IF (NULL == (str),  saret, SA3_NULLPTR_ERR);        \
				   nullch = (void*) memchr((const void*)(str), '\0', MAX_STR_LEN); \
				   GOTO_CLEANUP_IF (NULL == nullch, saret, SA3_PARAM_ERR);     }
#define CHK_NOTZERO(num, saret)  { GOTO_CLEANUP_IF (0    == num,    saret, SA3_PARAM_ERR);     }
#define CHK_GTZERO( num, saret)  { GOTO_CLEANUP_IF (0    >= num,    saret, SA3_PARAM_ERR);     }

#endif // BADARG_H
