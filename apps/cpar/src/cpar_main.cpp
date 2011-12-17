extern "C" {
#include "sa3parsers.h"
}

#include <iostream>
using namespace std;

int cparse (sa3cg _cg);

int main (int argc, char* argv[]) {
	int ret;
	sa3cg cg = NULL;
	if (2 == argc) {
		ret = sa3open_p(&cg, argv[1]);
		if (ret != 0) {
			cerr << "sa3open_p() failed with " << ret << endl;
		}
		ret = cparse(cg);
		// cerr << "cparse() returned " << ret << endl;
	} else {
		cerr << "usage: " << argv[0] << "graph; preprocessed code to stdin" << endl;
	}

cleanup:
	if (NULL != cg)
		sa3close_p(&cg);

	return ret;
}
