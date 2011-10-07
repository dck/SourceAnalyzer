#ifndef CG_H
#define CG_H

int adddeps (sa3cg  cg, char* cr, char* cd);
int opencg  (sa3cg* cg, const char* name, u_int32_t flags);
int closecg (sa3cg* cg);

#endif // CG_H
