#include <EXTERN.h>
#include <perl.h>

#include "jitapi.h"

#ifdef PERL_IMPLICIT_CONTEXT
#define jit_tTHX_ jit_type_void_ptr,
#define jit_tTHX  jit_type_void_ptr
#define jit_aTHX_ thx,
#define jit_aTHX  thx
#else
#define jit_tTHX_
#define jit_tTHX
#define jit_aTHX_
#define jit_aTHX
#endif

#define ARRAY(...) __VA_ARGS__
#define SIZE(a) (sizeof(a) / sizeof(a[0]))
#define DECLARE_SIGNATURE(name, ret, args) \
    jit_type_t _##name##_args[] = args; \
    jit_type_t _##name##_sig = jit_type_create_signature(jit_abi_cdecl, ret, _##name##_args, SIZE(_##name##_args), 1)

DECLARE_SIGNATURE(get_pad_sv, jit_type_void_ptr, ARRAY({jit_tTHX_ jit_type_nint}));

static SV *_get_pad_sv(pTHX_ jit_nint padix)
{
    return PAD_SVl(padix);
}

DECLARE_SIGNATURE(sv_2nv, jit_type_float64, ARRAY({jit_tTHX_ jit_type_void_ptr}));

static jit_float64 _sv_2nv(pTHX_ SV *sv)
{
    return SvNV(sv);
}

DECLARE_SIGNATURE(get_targ, jit_type_void_ptr, ARRAY({jit_tTHX}));

static SV *_get_targ(pTHX)
{
    return PAD_SV(PL_op->op_targ);
}

DECLARE_SIGNATURE(sv_set_iv, jit_type_void, ARRAY({jit_tTHX_ jit_type_void_ptr, jit_type_nint}));

static void _sv_set_iv(pTHX_ SV *sv, jit_nint iv)
{
    SvIV_set(sv, iv);
}

DECLARE_SIGNATURE(sv_set_nv, jit_type_void, ARRAY({jit_tTHX_ jit_type_void_ptr, jit_type_float64}));

static void _sv_set_nv(pTHX_ SV *sv, jit_float64 nv)
{
    sv_setnv(sv, nv);
}

DECLARE_SIGNATURE(push_sv, jit_type_void, ARRAY({jit_tTHX_ jit_type_void_ptr}));

static void _push_sv(pTHX_ SV *sv)
{
    dSP;
    XPUSHs(sv);
    PUTBACK;
}

DECLARE_SIGNATURE(get_op_next, jit_type_void_ptr, ARRAY({jit_tTHX}));

static OP *_get_op_next(pTHX)
{
    return PL_op->op_next;
}

DECLARE_SIGNATURE(trap, jit_type_void, ARRAY({jit_tTHX}));

static void _trap(pTHX)
{
    raise(SIGTRAP);
}

#define CALL(name, args) \
    jit_value_t _args[] = args; \
    return jit_insn_call_native(function, #name, (void *)_##name, _##name##_sig, _args, SIZE(_args), 0)

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t thx, jit_value_t padix)
{
    CALL(get_pad_sv, ARRAY({jit_aTHX_ padix}));
}

jit_value_t pa_sv_2nv(jit_function_t function, jit_value_t thx, jit_value_t sv)
{
    CALL(sv_2nv, ARRAY({jit_aTHX_ sv}));
}

jit_value_t pa_get_targ(jit_function_t function, jit_value_t thx)
{
    CALL(get_targ, ARRAY({jit_aTHX}));
}

jit_value_t pa_sv_set_iv(jit_function_t function, jit_value_t thx, jit_value_t sv, jit_value_t iv)
{
    CALL(sv_set_iv, ARRAY({jit_aTHX_ sv, iv}));
}

jit_value_t pa_sv_set_nv(jit_function_t function, jit_value_t thx, jit_value_t sv, jit_value_t nv)
{
    CALL(sv_set_nv, ARRAY({jit_aTHX_ sv, nv}));
}

jit_value_t pa_push_sv(jit_function_t function, jit_value_t thx, jit_value_t sv)
{
    CALL(push_sv, ARRAY({jit_aTHX_ sv}));
}

jit_value_t pa_get_op_next(jit_function_t function, jit_value_t thx)
{
    CALL(get_op_next, ARRAY({jit_aTHX}));
}

jit_value_t pa_trap(jit_function_t function, jit_value_t thx)
{
    CALL(trap, ARRAY({jit_aTHX}));
}
