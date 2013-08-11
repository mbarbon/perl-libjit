#include <EXTERN.h>
#include <perl.h>

#include "jitapi.h"

#define LIBJIT_THX_TYPE 4242

static SV *_pa_get_pad_sv(pTHX_ jit_nint padix)
{
    return PAD_SVl(padix);
}

static jit_float64 _pa_sv_2nv(pTHX_ SV *sv)
{
    return SvNV(sv);
}

static SV *_pa_get_targ(pTHX)
{
    return PAD_SV(PL_op->op_targ);
}

static void _pa_sv_set_iv(pTHX_ SV *sv, jit_nint iv)
{
    SvIV_set(sv, iv);
}

static void _pa_sv_set_nv(pTHX_ SV *sv, jit_float64 nv)
{
    sv_setnv(sv, nv);
}

static void _pa_push_sv(pTHX_ SV *sv)
{
    dSP;
    XPUSHs(sv);
    PUTBACK;
}

static OP *_pa_get_op_next(pTHX)
{
    return PL_op->op_next;
}

static void _pa_trap(pTHX)
{
    raise(SIGTRAP);
}

#include "gen-jitapi.inc"

#ifdef PERL_IMPLICIT_CONTEXT
static jit_type_t pp_signature[] = { jit_type_void_ptr };
#else
static jit_type_t pp_signature[] = {};
#endif

jit_function_t pa_create_pp(jit_context_t context)
{
    jit_type_t signature = jit_type_create_signature(
        jit_abi_cdecl, jit_type_void_ptr, pp_signature, SIZE(pp_signature), 0);
    jit_function_t pp = jit_function_create(context, signature);
#ifdef PERL_IMPLICIT_CONTEXT
    jit_value_t thx = jit_value_get_param(pp, 0);
    jit_function_set_meta(pp, LIBJIT_THX_TYPE, thx, 0, 1);
#endif

    return pp;
}
