#ifndef _LIBJIT_PERLAPI_H
#define _LIBJIT_PERLAPI_H

#include "jittypes.h"
#include "gen-jitapi.h"

jit_function_t pa_create_pp(jit_context_t context);
void pa_pp_op(jit_function_t function, OP *op);

#endif // _LIBJIT_PERLAPI_H
