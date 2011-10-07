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
void freekey    (char** pkey);
int  getstruct  (uint32* size, void** struc, uint8 cnt, ...);
void freestruct (void** struc);

#endif // GEN_H
