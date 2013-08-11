/* THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl */
/* DO NOT EDIT */

#ifndef _GEN_LIBJIT_PERLAPI_H
#define _GEN_LIBJIT_PERLAPI_H

#include <jit/jit.h>

#ifdef PERL_IMPLICIT_CONTEXT
#define jit_gTHX  ((jit_value_t) jit_function_get_meta(function, LIBJIT_THX_TYPE))
#define jit_tTHX  jit_type_void_ptr
#define jit_aTHX  jit_gTHX
#else
#define jit_tTHX
#define jit_aTHX
#endif

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t padix);
jit_value_t pa_sv_2nv(jit_function_t function, jit_value_t sv);
jit_value_t pa_get_targ(jit_function_t function);
jit_value_t pa_sv_set_iv(jit_function_t function, jit_value_t sv, jit_value_t iv);
jit_value_t pa_sv_set_nv(jit_function_t function, jit_value_t sv, jit_value_t nv);
jit_value_t pa_push_sv(jit_function_t function, jit_value_t sv);
jit_value_t pa_get_op_next(jit_function_t function);
jit_value_t pa_trap(jit_function_t function);

#endif // _GEN_LIBJIT_PERLAPI_H
