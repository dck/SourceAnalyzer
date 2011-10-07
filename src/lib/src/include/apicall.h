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

#ifndef APICALL_H
#define APICALL_H

/* should be function specific, in other words __func__ name must
 be a part of lable */
#define CLEANUP cleanup##__func__

#ifdef TRACE
	#define TRACE_BEFORE_CALL (call)       fprintf (stderr, "---> %s\n", #call)
	#define TRACE_AFTER_CALL  (call, sts)  fprintf (stderr, "%d <- %s\n", sts, #call)
#else
	#define TRACE_BEFORE_CALL (call)
	#define TRACE_AFTER_CALL  (call, sts)
#endif

#define GOTO_CLEANUP_IF (cond, saret, err)                           \
	if (cond) {                                                  \
	#ifdef ERROR_LOG                                             \
		fprintf (stderr, "error: %s: %s: %l: code: %d\n",    \
			 __FILE__, __func__, __LINE__, (err));       \
	#endif                                                       \
		saret = (err);                                       \
		goto CLEANUP;                                        \
	}

#define CALL_API (sts, call)    { TRACE_BEFORE_CALL (call);             \
				  sts = call;                           \
				  TRACE_AFTER_CALL  (call, sts);        \
				  GOTO_CLEANUP_IF (0 != sts, sts, sts); }

#define CALL_BD_API (sts, call) { int __tmp = 0;                                 \
				  TRACE_BEFORE_CALL (call);                      \
				  __tmp = call;                                  \
				  TRACE_BEFORE_CALL (call, __tmp);               \
				  GOTO_CLEANUP_IF (0 != __tmp && DB_NOTFOUND != __tmp, sts, SA3_DB_ERR); }

#endif // APICALL_H
