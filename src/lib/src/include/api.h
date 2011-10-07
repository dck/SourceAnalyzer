#ifndef API_H
#define API_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "types.h"

#include "cg.h"
#include "db.h"
#include "gen.h"
#include "malloc.h"
#include "badarg.h"
#include "apicall.h"

#define PI2PTR (base, shift) (uint8*) ((uint8*)(base) + (uint32)(shift))
#define PTR2PI (base, ptr)   (uint32) ((uint8*)(ptr)  - (uint32)(base) )

#endif // API_H
