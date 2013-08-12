/* THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl */
/* DO NOT EDIT */

#define SIZE(a) (sizeof(a) / sizeof(a[0]))

static jit_type_t _pa_get_pad_sv_parms[] = {jit_tTHX, jit_type_nint};
static jit_type_t _pa_get_pad_sv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _pa_get_pad_sv_parms, SIZE(_pa_get_pad_sv_parms), 1);

jit_value_t pa_get_pad_sv(jit_function_t function, jit_value_t padix)
{
    jit_value_t _args[] = {jit_aTHX, padix};
    return jit_insn_call_native(function, "pa_get_pad_sv", (void *)_pa_get_pad_sv, _pa_get_pad_sv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_sv_nv_parms[] = {jit_tTHX, jit_type_void_ptr};
static jit_type_t _pa_sv_nv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_float64, _pa_sv_nv_parms, SIZE(_pa_sv_nv_parms), 1);

jit_value_t pa_sv_nv(jit_function_t function, jit_value_t sv)
{
    jit_value_t _args[] = {jit_aTHX, sv};
    return jit_insn_call_native(function, "pa_sv_nv", (void *)_pa_sv_nv, _pa_sv_nv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_sv_2mortal_parms[] = {jit_tTHX, jit_type_void_ptr};
static jit_type_t _pa_sv_2mortal_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _pa_sv_2mortal_parms, SIZE(_pa_sv_2mortal_parms), 1);

jit_value_t pa_sv_2mortal(jit_function_t function, jit_value_t sv)
{
    jit_value_t _args[] = {jit_aTHX, sv};
    return jit_insn_call_native(function, "pa_sv_2mortal", (void *)_pa_sv_2mortal, _pa_sv_2mortal_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_new_sv_nv_parms[] = {jit_tTHX, jit_type_float64};
static jit_type_t _pa_new_sv_nv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _pa_new_sv_nv_parms, SIZE(_pa_new_sv_nv_parms), 1);

jit_value_t pa_new_sv_nv(jit_function_t function, jit_value_t nv)
{
    jit_value_t _args[] = {jit_aTHX, nv};
    return jit_insn_call_native(function, "pa_new_sv_nv", (void *)_pa_new_sv_nv, _pa_new_sv_nv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_get_targ_parms[] = {jit_tTHX};
static jit_type_t _pa_get_targ_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _pa_get_targ_parms, SIZE(_pa_get_targ_parms), 1);

jit_value_t pa_get_targ(jit_function_t function)
{
    jit_value_t _args[] = {jit_aTHX};
    return jit_insn_call_native(function, "pa_get_targ", (void *)_pa_get_targ, _pa_get_targ_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_sv_set_iv_parms[] = {jit_tTHX, jit_type_void_ptr, jit_type_nint};
static jit_type_t _pa_sv_set_iv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, _pa_sv_set_iv_parms, SIZE(_pa_sv_set_iv_parms), 1);

jit_value_t pa_sv_set_iv(jit_function_t function, jit_value_t sv, jit_value_t iv)
{
    jit_value_t _args[] = {jit_aTHX, sv, iv};
    return jit_insn_call_native(function, "pa_sv_set_iv", (void *)_pa_sv_set_iv, _pa_sv_set_iv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_sv_set_nv_parms[] = {jit_tTHX, jit_type_void_ptr, jit_type_float64};
static jit_type_t _pa_sv_set_nv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, _pa_sv_set_nv_parms, SIZE(_pa_sv_set_nv_parms), 1);

jit_value_t pa_sv_set_nv(jit_function_t function, jit_value_t sv, jit_value_t nv)
{
    jit_value_t _args[] = {jit_aTHX, sv, nv};
    return jit_insn_call_native(function, "pa_sv_set_nv", (void *)_pa_sv_set_nv, _pa_sv_set_nv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_push_sv_parms[] = {jit_tTHX, jit_type_void_ptr};
static jit_type_t _pa_push_sv_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, _pa_push_sv_parms, SIZE(_pa_push_sv_parms), 1);

jit_value_t pa_push_sv(jit_function_t function, jit_value_t sv)
{
    jit_value_t _args[] = {jit_aTHX, sv};
    return jit_insn_call_native(function, "pa_push_sv", (void *)_pa_push_sv, _pa_push_sv_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_get_op_next_parms[] = {jit_tTHX};
static jit_type_t _pa_get_op_next_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void_ptr, _pa_get_op_next_parms, SIZE(_pa_get_op_next_parms), 1);

jit_value_t pa_get_op_next(jit_function_t function)
{
    jit_value_t _args[] = {jit_aTHX};
    return jit_insn_call_native(function, "pa_get_op_next", (void *)_pa_get_op_next, _pa_get_op_next_sig, _args, SIZE(_args), 0);
}

static jit_type_t _pa_trap_parms[] = {jit_tTHX};
static jit_type_t _pa_trap_sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, _pa_trap_parms, SIZE(_pa_trap_parms), 1);

jit_value_t pa_trap(jit_function_t function)
{
    jit_value_t _args[] = {jit_aTHX};
    return jit_insn_call_native(function, "pa_trap", (void *)_pa_trap, _pa_trap_sig, _args, SIZE(_args), 0);
}
