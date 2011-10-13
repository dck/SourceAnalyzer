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

extern "C" {
#include "sa3parsers.h"
#include "sa3tools.h"
}

#include <iostream>

using namespace std;

extern "C" {
int linkcg (char* rc, char** sr, int num);
}

int main (int argc, char* argv[]) {
	int sts = -1;
	
	if (2 == argc) {
		int ret = 0;
		sa3cg r = NULL;
		string str;

		ret = sa3open_p(&r, argv[1]);
		while (0 == ret && getline (cin, str)) {
			cout << "linking " << str << endl;
			sa3cg s = NULL;
			ret = sa3open_t(&s, str.c_str());
			if (0 != ret) break;
			ret = sa3mergecalls(r, s);
			if (0 != ret) break;
			ret = sa3mergedecls(r, s);
			if (0 != ret) break;
			ret = sa3close_t(&s);
		}
		if (NULL != r) {
			ret = sa3close_p(&r);
		}
		if (0 == ret)
			sts = 0;
	} else if (2 < argc)
		sts = linkcg (argv[1], &(argv[2]), argc-2);
	else
		cerr << "usage: " << argv[0] << " newgraph graph1 [graph2...]" << endl;
	
	if (0 != sts) {
		cerr << "linkage failed with " << sts << endl;
	}

	return sts;
}
