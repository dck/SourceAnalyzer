#include <db.h>

int getbypref     (DB*  pdb, char* pref, uint16 preflen, sa3list* lst);
int getbykey      (DB*  pdb, char* key,  uint16 keylen,  sa3list* lst);
int getfirstbykey (DB*  pdb, void* key,  uint16 keylen,  void** data, uint32* datalen);
int put2db        (DB*  pdb, void* key,  uint16 keylen,  void*  data, uint32  datalen);
int opendb        (DB** ppdb, u_int32_t flags);
int closedb       (sa3cg cg);
