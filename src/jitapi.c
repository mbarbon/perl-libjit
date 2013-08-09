#include <EXTERN.h>
#include <perl.h>

#include "gen-jitapi.h"

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
