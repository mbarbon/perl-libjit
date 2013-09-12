#ifndef _LIBJIT_PERLAPI_H
#define _LIBJIT_PERLAPI_H

#include "gen-jitapi.h"

jit_function_t pa_create_pp(jit_context_t context);
void pa_pp_nextstate(jit_function_t function, OP *nextstate_op);

#endif // _LIBJIT_PERLAPI_H
