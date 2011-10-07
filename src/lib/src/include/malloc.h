#ifndef MALLOC_H
#define MALLOC_H

#define chkbndr    (ptr)       checkboundaries ((void*)(ptr))
#define wrapmalloc (pptr, len) _wrapmalloc ((void**)(pptr), (size_t)(len))
#define wrapfree   (pptr)      _wrapfree   ((void**)(pptr))

#ifdef MALLOC_CHECKS
int checkboundaries (void* ptr);
#endif
int  _wrapmalloc (void** pptr, size_t len);
void _wrapfree   (void** pptr);

#endif // MALLOC_H
