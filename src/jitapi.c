#include <EXTERN.h>
#include <perl.h>

#include "jitapi.h"

/* From perl, not super-public, I suppose, but installed alongside the other
 * perl headers. */
#include <pp_proto.h>

#define LIBJIT_THX_TYPE 4242
#define SIZE(a) (sizeof(a) / sizeof(a[0]))

static jit_type_t _void__athx_parms[] = {jit_tTHX};
static jit_type_t _voidptr__athx_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _void__athx_parms, SIZE(_void__athx_parms), 1);

inline jit_value_t make_u32(jit_function_t function, U32 value)
{
    return jit_value_create_nint_constant(function, jit_type_uint, value);
}

inline jit_value_t make_i32(jit_function_t function, U32 value)
{
    return jit_value_create_nint_constant(function, jit_type_int, value);
}

#define OFF(type, name) PTR2IV(&((type *)0)->name)

#ifdef USE_ITHREADS
#define IOFF(name) PTR2IV(&((PerlInterpreter *)0)->I##name)

inline jit_value_t get_ivar_address(jit_function_t function, jit_nint offset)
{
    jit_value_t thx = jit_gTHX;

    return jit_insn_add_relative(function, thx, offset);
}

inline jit_value_t get_ivar(jit_function_t function, jit_nint offset, jit_type_t type)
{
    jit_value_t thx = jit_gTHX;

    return jit_insn_load_relative(function, thx, offset, type);
}

inline int set_ivar(jit_function_t function, jit_nint offset, jit_value_t value)
{
    jit_value_t thx = jit_gTHX;

    return jit_insn_store_relative(function, thx, offset, value);
}

#define IVAR(name, type) get_ivar(function, IOFF(name), type)
#define IVAR_address(name) get_ivar_address(function, IOFF(name))
#define IVAR_set(name, value) set_ivar(function, IOFF(name), value)
#else
inline jit_value_t get_ivar_address(jit_function_t function, void *address)
{
    jit_constant_t c;
    c.type = jit_type_void_ptr;
    c.un.ptr_value = address;
    return jit_value_create_constant(function, &c);
}

inline jit_value_t get_ivar(jit_function_t function, void *address, jit_type_t type)
{
    jit_constant_t c;
    c.type = jit_type_void_ptr;
    c.un.ptr_value = address;
    jit_value_t addr = jit_value_create_constant(function, &c);
    return jit_insn_load_relative(function, addr, 0, type);
}

inline int set_ivar(jit_function_t function, void *address, jit_value_t value)
{
    jit_constant_t c;
    c.type = jit_type_void_ptr;
    c.un.ptr_value = address;
    jit_value_t addr = jit_value_create_constant(function, &c);
    return jit_insn_store_relative(function, addr, 0, value);
}

#define IVAR(name, type) get_ivar(function, &PL_##name, type)
#define IVAR_address(name) get_ivar_address(function, &PL_##name)
#define IVAR_set(name, value) set_ivar(function, &PL_##name, value)
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

jit_value_t pa_sv_yes(jit_function_t function)
{
    return IVAR_address(sv_yes);
}

jit_value_t pa_sv_no(jit_function_t function)
{
    return IVAR_address(sv_no);
}

// void Perl_save_clearsv(pTHX_ SV **svp) => pa_save_clearsv

static void _pa_pp_enterloop(pTHX)
{
    // enterloop only uses PL_op to return op_next, which we ignore
    Perl_pp_enterloop(aTHX);
}

static void _pa_pp_leaveloop(pTHX)
{
    // leaveloop only uses PL_op to return op_next, which we ignore
    Perl_pp_leaveloop(aTHX);
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

static AV *_pa_gv_avn(pTHX_ GV *gv)
{
    return GvAVn(gv);
}

static HV *_pa_gv_hv(pTHX_ GV *gv)
{
    return GvHV(gv);
}

jit_value_t pa_sv_any(jit_function_t function, jit_value_t sv)
{
    return jit_insn_load_relative(function, sv, OFF(SV, sv_any), jit_type_void_ptr);
}

jit_value_t pa_sv_flags(jit_function_t function, jit_value_t sv)
{
    return jit_insn_load_relative(function, sv, OFF(SV, sv_flags), jit_type_uint);
}

jit_value_t pa_sv_nvx(jit_function_t function, jit_value_t sv)
{
    jit_value_t any = pa_sv_any(function, sv);
    return jit_insn_load_relative(function, any, OFF(XPVNV, xnv_u.xnv_nv), jit_type_NV);
}

// NV Perl_sv_2nv_flags(pTHX_ SV *sv, I32 flags) => pa_sv_2nv_flags

jit_value_t pa_sv_2nv(jit_function_t function, jit_value_t sv)
{
    return pa_sv_2nv_flags(function, sv, make_i32(function, SV_GMAGIC));
}

jit_value_t pa_sv_nok_nog(jit_function_t function, jit_value_t sv)
{
    jit_value_t flags = pa_sv_flags(function, sv);

    jit_value_t masked = jit_insn_and(function, flags, make_u32(function, SVf_NOK|SVs_GMG));

    return jit_insn_eq(function, masked, make_u32(function, SVf_NOK));
}

jit_value_t pa_sv_nv(jit_function_t function, jit_value_t sv)
{
    jit_label_t is_nok = jit_label_undefined, end = jit_label_undefined;
    jit_value_t nok_nog = pa_sv_nok_nog(function, sv);
    jit_value_t result = jit_value_create(function, jit_type_NV);

    jit_insn_branch_if(function, nok_nog, &is_nok);

    jit_insn_store(function, result, pa_sv_2nv(function, sv));
    jit_insn_branch(function, &end);

    jit_insn_label(function, &is_nok);
    jit_insn_store(function, result, pa_sv_nvx(function, sv));

    jit_insn_label(function, &end);

    return result;
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

static SV *_pa_av_fetch(pTHX_ AV *av, IV index)
{
    SV **svp;
    svp = av_fetch(av, index, 0);
    if (svp == NULL)
      return &PL_sv_undef;
    return *svp;
}

static SV *_pa_av_fetch_lvalue(pTHX_ AV *av, IV index)
{
    SV **svp;
    svp = av_fetch(av, index, 1);
    if (svp == NULL)
      return &PL_sv_undef;
    return *svp;
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

#include "gen-jitapi.inc"

jit_function_t pa_create_pp(jit_context_t context) /* no autogen wrapper */
{
    jit_function_t pp = jit_function_create(context, _voidptr__athx_sig);
#ifdef PERL_IMPLICIT_CONTEXT
    jit_value_t thx = jit_value_get_param(pp, 0);
    jit_function_set_meta(pp, LIBJIT_THX_TYPE, thx, 0, 1);
#endif

    return pp;
}

void pa_pp_op(jit_function_t function, OP *op) /* no autogen wrapper */
{
    // JIT constant that points at the OP that we already
    // know about at JIT compile time
    jit_constant_t c;
    c.type = jit_type_void_ptr;
    c.un.ptr_value = (void *)op;
    jit_value_t op_addr = jit_value_create_constant(function, &c);

    // Remember old PL_op value
    jit_value_t oldop = IVAR(op, jit_type_void_ptr);
    // Overwrite it with OP
    IVAR_set(op, op_addr);

    // Invoke
    jit_value_t args[] = {jit_aTHX};
    jit_insn_call_native(function, "pa_pp_op", (void *)op->op_ppaddr, _voidptr__athx_sig, args, SIZE(args), 0);

    // Reset
    IVAR_set(op, oldop);
}

