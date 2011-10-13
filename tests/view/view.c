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

static int showcg (char* act, char* file, char* func) {
	int ret = 0;
	sa3cg cg = NULL;
	sa3list lst = NULL;
	sa3listit it = NULL;

	CALL_API(ret, sa3open_t(&cg, file));
	if (0 == strcmp(act, "cld")) {
		CALL_API(ret, sa3getclds(cg, func, &lst));
	} else if (0 == strcmp(act, "prn")) {
		CALL_API(ret, sa3getprns(cg, func, &lst));
	} else if (0 == strcmp(act, "lst")) {
		CALL_API(ret, sa3getfuncs(cg, &lst));
	} else {
		ret = -1;
		goto CLEANUP;
	}

	SA3_FORLST(it, lst) {
		printf ("%s\n", (char*) sa3resolveit(lst, it));
	}

CLEANUP:
	if (NULL != lst) {
		// sa3freelist(&lst);
	}
	if (NULL != cg) {
		CALL_API(ret, sa3close_t(&cg));
	}
	return ret;
}

int main (int argc, char* argv[]) {
	int sts = -1;
	char* func = NULL;

	if (4 == argc)
		func = argv[3];

	if (2 < argc)
		sts = showcg (argv[1], argv[2], func);


	if (-1 == sts)
		fprintf (stderr, "usage: %s cld|prn|lst graph [func]\n", argv[0]);

	return sts;
}

