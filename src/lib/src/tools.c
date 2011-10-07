#include "include/api.h"

int sa3open_t  (sa3cg* cg, const char* name) {
	int saret = 0;

	CHK_PTR (cg,   saret);
	CHK_CG  (*cg,  saret);
	CHK_STR (name, saret);

	CALL_API(saret, opencg (cg, name, DB_RDONLY));

CLEANUP:
	return saret;
}

int sa3close_t (sa3cg* cg) {
	int saret = 0;

	CHK_PTR (cg,  saret);
	CHK_CG  (*cg, saret);

	CALL_API(saret, closecg (cg));

CLEANUP:
	return saret;
}

int sa3getclds (sa3cg cg, char* func, sa3list* list) {
	uint32 len   = 0; // FK: assume this parameter is redundant
	int    saret = 0;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);
	// CHK_PTR (!(*list), saret); // FK: not sure if this is needed.

	CALL_API (saret, getfirstbykey (cg, "cld ", func, list, &len));

CLEANUP:
	return saret
}

int sa3getprns (sa3cg cg, char* func, sa3list* list) {
	uint32 len   = 0; // FK: assume this parameter is redundant
	int    saret = 0;

	CHK_CG  (cg,   saret);
	CHK_PTR (func, saret);
	CHK_PTR (list, saret);
	// CHK_PTR (!(*list), saret); // FK: not sure if this is needed.

	CALL_API (saret, getfirstbykey (cg, "prn ", func, list, &len));

CLEANUP:
	return saret;
}

void sa3freelist (sa3list* plist) {
	if (NULL != pplist) {
		sa3list list = *plist;
		if (NULL != list) {
			if (NULL != list->cur) {
				list->cur->close (list->cur);
				list->cur = NULL;
			}
			wrapfree (&list);
		}
		*plist = list;
	}
}

sa3listit sa3firstlstit (sa3list lst) {
	return (sa3listit)(lst->lst);
}

sa3listit sa3nextlstit  (sa3list lst, sa3listit it) {
	return (sa3listit)(it++);
}

sa3listit sa3lastlstit  (sa3list lst) {
	return (sa3listit)(lst->lst + lst->num - 1);
}

void* sa3resolveit (sa3list lst, sa3listit it) {
	void* ptr = NULL;

	// Depending on what function returned the list,
	// itterator will be properly resolved below.
	if (NULL != lst->cur) {
		// To Be Implemented.
		ptr = (void*)(*it);
	} else { // obtained thru sa3getclds() or sa3getprns() call.
		ptr = (void*)PI2PTR(lst, (*it));
	}

	return (void*)PI2PTR(lst, (*it));
}
