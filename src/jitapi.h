#ifndef _LIBJIT_PERLAPI_H
#define _LIBJIT_PERLAPI_H

#include <jit/jit.h>

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t thx, jit_value_t padix);

jit_value_t pa_sv_2nv(jit_function_t function, jit_value_t thx, jit_value_t sv);

jit_value_t pa_get_targ(jit_function_t function, jit_value_t thx);

jit_value_t pa_sv_set_iv(jit_function_t function, jit_value_t thx, jit_value_t sv, jit_value_t iv);
jit_value_t pa_sv_set_nv(jit_function_t function, jit_value_t thx, jit_value_t sv, jit_value_t nv);

jit_value_t pa_push_sv(jit_function_t function, jit_value_t thx, jit_value_t sv);

jit_value_t pa_get_op_next(jit_function_t function, jit_value_t thx);

jit_value_t pa_trap(jit_function_t function, jit_value_t thx);

#endif // _LIBJIT_PERLAPI_H
