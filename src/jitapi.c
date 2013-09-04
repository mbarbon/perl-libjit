#include <EXTERN.h>
#include <perl.h>

#include "jitapi.h"
#include "jit_perl_typemapping.h"

#define LIBJIT_THX_TYPE 4242

static SV *_pa_get_pad_sv(pTHX_ jit_nint padix)
{
    return PAD_SVl(padix);
}

static SV *_pa_gv_sv(pTHX_ GV *gv)
{
    return GvSV(gv);
}

static SV *_pa_gv_svn(pTHX_ GV *gv)
{
    return GvSVn(gv);
}

static AV *_pa_gv_av(pTHX_ GV *gv)
{
    return GvAV(gv);
}

static HV *_pa_gv_hv(pTHX_ GV *gv)
{
    return GvHV(gv);
}

static jit_NV _pa_sv_nv(pTHX_ SV *sv)
{
    return SvNV(sv);
}

static jit_IV _pa_sv_iv(pTHX_ SV *sv)
{
    return SvIV(sv);
}

static jit_UV _pa_sv_uv(pTHX_ SV *sv)
{
    return SvUV(sv);
}

static SV *_pa_sv_2mortal(pTHX_ SV *sv)
{
    return sv_2mortal(sv);
}

static sv *_pa_new_sv_nv(pTHX_ jit_NV nv)
{
    return newSVnv(nv);
}

static SV *_pa_new_sv_iv(pTHX_ jit_IV iv)
{
    return newSViv(iv);
}

static SV *_pa_new_sv_uv(pTHX_ jit_UV uv)
{
    return newSVuv(uv);
}

static SV *_pa_new_mortal_sv(pTHX_ jit_UV uv)
{
    return sv_newmortal();
}

static SV *_pa_get_targ(pTHX)
{
    return PAD_SV(PL_op->op_targ);
}

static jit_IV _pa_sv_true(pTHX_ SV *sv)
{
    return SvTRUE(sv);
}

static void _pa_sv_set_iv(pTHX_ SV *sv, jit_IV iv)
{
    sv_setiv(sv, iv);
}

static void _pa_sv_set_nv(pTHX_ SV *sv, jit_NV nv)
{
    sv_setnv(sv, nv);
}

static void _pa_push_sv(pTHX_ SV *sv)
{
    dSP;
    XPUSHs(sv);
    PUTBACK;
}

static void _pa_call_runloop(pTHX_ OP *op)
{
    OP *oldop = PL_op;
    PL_op = op;
    CALLRUNOPS(aTHX);
    PL_op = oldop;
}

static SV *_pa_pop_sv(pTHX)
{
    dSP;
    SV *sv = POPs;
    PUTBACK;
    return sv;
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
