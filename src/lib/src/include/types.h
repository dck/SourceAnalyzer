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
typedef void* sa3listit;

struct _dep {
	char*           func;
	uint16          flen;
	uint32          fid;
	struct _dep**   dpds;
	uint32          dpdsnum;
	uint32          bfsize;
};
typedef struct _dep dep;

struct cg {
	DB*     db;
	dep*    cld;
	dep*    prn;
	uint32  fnnum;
	uint32  bfsize;
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
