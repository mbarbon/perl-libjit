#include <EXTERN.h>
#include <perl.h>

#include "jitapi.h"
#include "jit_perl_typemapping.h"

/* From perl, not super-public, I suppose, but installed alongside the other
 * perl headers. */
#include <pp_proto.h>

#define LIBJIT_THX_TYPE 4242

#ifdef USE_ITHREADS
#define IOFF(name) PTR2IV(&((PerlInterpreter *)0)->I##name)

inline jit_value_t get_ivar(jit_function_t function, jit_nint offset, jit_type_t type)
{
    jit_value_t thx = jit_gTHX;

    return jit_insn_load_relative(function, thx, offset, type);
}

#define IVAR(name, type) get_ivar(function, IOFF(name), type)
#else
inline jit_value_t get_ivar(jit_function_t function, void *address, jit_type_t type)
{
    jit_constant_t c;
    c.type = jit_type_void_ptr;
    c.un.ptr_value = address;
    jit_value_t addr = jit_value_create_constant(function, &c);
    return jit_insn_load_relative(function, addr, 0, type);
}

#define IVAR(name, type) get_ivar(function, &PL_##name, type)
#endif

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t padix)
{
    jit_value_t curpad = IVAR(curpad, jit_type_void_ptr);

    return jit_insn_load_elem(function, curpad, padix, jit_type_void_ptr);
}

jit_value_t pa_get_pad_sv_address(jit_function_t function, jit_value_t padix)
{
    jit_value_t curpad = IVAR(curpad, jit_type_void_ptr);

    return jit_insn_load_elem_address(function, curpad, padix, jit_type_void_ptr);
}

// void Perl_save_clearsv(pTHX_ SV **svp) => pa_save_clearsv

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

// SV *Perl_sv_2mortal(pTHX_ SV *sv) => pa_sv_2mortal
// SV *Perl_newSVnv(pTHX_ jit_NV nv) => pa_new_sv_nv
// SV *Perl_newSViv(pTHX_ jit_IV iv) => pa_new_sv_iv
// SV *Perl_newSVuv(pTHX_ jit_UV uv) => pa_new_sv_uv
// SV *Perl_sv_newmortal(pTHX) => pa_new_mortal_sv

static SV *_pa_get_targ(pTHX)
{
    return PAD_SV(PL_op->op_targ);
}

static jit_IV _pa_sv_true(pTHX_ SV *sv)
{
    return SvTRUE(sv);
}

// void Perl_sv_setiv(pTHX_ SV *sv, jit_IV iv) => pa_sv_set_iv
// void Perl_sv_setnv(pTHX_ SV *sv, jit_NV nv) => pa_sv_set_nv

static void _pa_sv_set_sv_nosteal(pTHX_ SV *dsv, SV *ssv)
{
    SvSetSV_nosteal(dsv, ssv);
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

static void _pa_pp_nextstate(pTHX)
{
    (void)Perl_pp_nextstate(aTHX);
}

#include "gen-jitapi.inc"

#ifdef PERL_IMPLICIT_CONTEXT
static jit_type_t pp_signature[] = { jit_type_void_ptr };
#else
static jit_type_t pp_signature[] = {};
#endif

jit_function_t pa_create_pp(jit_context_t context) /* no autogen wrapper */
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
