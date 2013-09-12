/* THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl */
/* DO NOT EDIT */

#ifndef _GEN_LIBJIT_PERLAPI_H
#define _GEN_LIBJIT_PERLAPI_H

#include <jit/jit.h>

#ifdef PERL_IMPLICIT_CONTEXT
#define jit_gTHX  ((jit_value_t) jit_function_get_meta(function, LIBJIT_THX_TYPE))
#define jit_tTHX  jit_type_void_ptr
#define jit_aTHX  jit_gTHX
#define jit_tTHX_  jit_type_void_ptr,
#define jit_aTHX_  jit_gTHX,
#else
#define jit_tTHX
#define jit_aTHX
#define jit_tTHX_
#define jit_aTHX_
#endif

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t padix);
jit_value_t pa_get_pad_sv_address(jit_function_t function, jit_value_t padix);
jit_value_t pa_save_clearsv(jit_function_t function, jit_value_t svp);
jit_value_t pa_gv_sv(jit_function_t function, jit_value_t gv);
jit_value_t pa_gv_svn(jit_function_t function, jit_value_t gv);
jit_value_t pa_gv_av(jit_function_t function, jit_value_t gv);
jit_value_t pa_gv_hv(jit_function_t function, jit_value_t gv);
jit_value_t pa_sv_nv(jit_function_t function, jit_value_t sv);
jit_value_t pa_sv_iv(jit_function_t function, jit_value_t sv);
jit_value_t pa_sv_uv(jit_function_t function, jit_value_t sv);
jit_value_t pa_sv_2mortal(jit_function_t function, jit_value_t sv);
jit_value_t pa_new_sv_nv(jit_function_t function, jit_value_t nv);
jit_value_t pa_new_sv_iv(jit_function_t function, jit_value_t iv);
jit_value_t pa_new_sv_uv(jit_function_t function, jit_value_t uv);
jit_value_t pa_new_mortal_sv(jit_function_t function);
jit_value_t pa_get_targ(jit_function_t function);
jit_value_t pa_sv_true(jit_function_t function, jit_value_t sv);
jit_value_t pa_sv_set_iv(jit_function_t function, jit_value_t sv, jit_value_t iv);
jit_value_t pa_sv_set_nv(jit_function_t function, jit_value_t sv, jit_value_t nv);
jit_value_t pa_sv_set_sv_nosteal(jit_function_t function, jit_value_t dsv, jit_value_t ssv);
jit_value_t pa_push_sv(jit_function_t function, jit_value_t sv);
jit_value_t pa_call_runloop(jit_function_t function, jit_value_t op);
jit_value_t pa_pop_sv(jit_function_t function);
jit_value_t pa_get_op_next(jit_function_t function);
jit_value_t pa_trap(jit_function_t function);

#endif // _GEN_LIBJIT_PERLAPI_H
