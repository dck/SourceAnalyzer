#ifndef BADARG_H
#define BADARG_H

/* FK: why 300? This is SPARTA? */
#define MAX_STR_LEN  (300)

#define CHK_PTR     (ptr, saret) { GOTO_CLEANUP_IF (NULL == (ptr),    saret, SA3_NULLPTR_ERR); }
#define CHK_CG      (cg,  saret) { GOTO_CLEANUP_IF (NULL == (cg),     saret, SA3_NULLPTR_ERR); \
				   GOTO_CLEANUP_IF (NULL == (cg)->db, saret, SA3_NULLPTR_ERR); }
#define CHK_STR     (str, saret) { void* nullch = NULL;                                        \
				   GOTO_CLEANUP_IF (NULL == (str),  saret, SA3_NULLPTR_ERR);   \
				   nullch = memchcr((const void*)(str), '\0', MAX_STR_LEN);    \
				   GOTO_CLEANUP_IF (NULL == nullch, saret, SA3_PARAM_ERR);     }
#define CHK_NOTZERO (num, saret) { GOTO_CLEANUP_IF (0    == num,    saret, SA3_PARAM_ERR);     }

#endif // BADARG_H
