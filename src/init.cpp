#include "helpers.h"
#include "jit_perl_typemapping.h"

#define DEFINE_TYPE(name) \
    lj_define_type(aTHX_ jit_type_##name, "jit_type_" #name)

#define DEFINE_INT(name) \
    lj_define_int(aTHX_ name, #name)

void lj_define_int(pTHX_ IV value, const char *name)
{
    char buffer[64];

    strcpy(buffer, "LibJIT::API::");
    strcat(buffer, name);

    SV *sv = get_sv(buffer, 1);
    HV *hv = gv_stashpvs("LibJIT::API", 1);

    sv_setiv(sv, value);
    newCONSTSUB(hv, name, sv);
}

void lj_define_type(pTHX_ jit_type_t type, const char *name)
{
    char buffer[64];

    strcpy(buffer, "LibJIT::API::");
    strcat(buffer, name);

    SV *sv = sv_setref_pv(get_sv(buffer, 1), "LibJIT::Type", type);
    HV *hv = gv_stashpvs("LibJIT::API", 1);

    newCONSTSUB(hv, name, sv);
}

void lj_define_instance(pTHX_ void *thing, const char *perl_name, const char *perl_class)
{
    char buffer[64];

    strcpy(buffer, "LibJIT::API::");
    strcat(buffer, perl_name);

    SV *sv = sv_setref_pv(get_sv(buffer, 1), perl_class, thing);
    HV *hv = gv_stashpvs("LibJIT::API", 1);

    newCONSTSUB(hv, perl_name, sv);
}

void lj_define_types(pTHX)
{
    DEFINE_TYPE(void);
    DEFINE_TYPE(sbyte);
    DEFINE_TYPE(ubyte);
    DEFINE_TYPE(short);
    DEFINE_TYPE(ushort);
    DEFINE_TYPE(int);
    DEFINE_TYPE(uint);
    DEFINE_TYPE(nint);
    DEFINE_TYPE(nuint);
    DEFINE_TYPE(long);
    DEFINE_TYPE(ulong);
    DEFINE_TYPE(float32);
    DEFINE_TYPE(float64);
    DEFINE_TYPE(nfloat);
    DEFINE_TYPE(void_ptr);

    DEFINE_TYPE(sys_bool);
    DEFINE_TYPE(sys_char);
    DEFINE_TYPE(sys_schar);
    DEFINE_TYPE(sys_uchar);
    DEFINE_TYPE(sys_short);
    DEFINE_TYPE(sys_ushort);
    DEFINE_TYPE(sys_int);
    DEFINE_TYPE(sys_uint);
    DEFINE_TYPE(sys_long);
    DEFINE_TYPE(sys_ulong);
    DEFINE_TYPE(sys_longlong);
    DEFINE_TYPE(sys_ulonglong);
    DEFINE_TYPE(sys_float);
    DEFINE_TYPE(sys_double);
    DEFINE_TYPE(sys_long_double);

    DEFINE_TYPE(NV);
    DEFINE_TYPE(IV);
    DEFINE_TYPE(UV);
}

void lj_define_constants(pTHX)
{
    DEFINE_INT(jit_abi_cdecl);
    DEFINE_INT(jit_abi_vararg);
    DEFINE_INT(jit_abi_stdcall);
    DEFINE_INT(jit_abi_fastcall);

    lj_define_instance(aTHX_ (void *)jit_label_undefined, "jit_label_undefined", "LibJIT::Label");

    DEFINE_INT(JIT_TYPE_INVALID);
    DEFINE_INT(JIT_TYPE_VOID);
    DEFINE_INT(JIT_TYPE_SBYTE);
    DEFINE_INT(JIT_TYPE_UBYTE);
    DEFINE_INT(JIT_TYPE_SHORT);
    DEFINE_INT(JIT_TYPE_USHORT);
    DEFINE_INT(JIT_TYPE_INT);
    DEFINE_INT(JIT_TYPE_UINT);
    DEFINE_INT(JIT_TYPE_NINT);
    DEFINE_INT(JIT_TYPE_NUINT);
    DEFINE_INT(JIT_TYPE_LONG);
    DEFINE_INT(JIT_TYPE_ULONG);
    DEFINE_INT(JIT_TYPE_FLOAT32);
    DEFINE_INT(JIT_TYPE_FLOAT64);
    DEFINE_INT(JIT_TYPE_NFLOAT);
    DEFINE_INT(JIT_TYPE_MAX_PRIMITIVE);
    DEFINE_INT(JIT_TYPE_STRUCT);
    DEFINE_INT(JIT_TYPE_UNION);
    DEFINE_INT(JIT_TYPE_SIGNATURE);
    DEFINE_INT(JIT_TYPE_PTR);
    DEFINE_INT(JIT_TYPE_FIRST_TAGGED);

    DEFINE_INT(JIT_TYPETAG_NAME);
    DEFINE_INT(JIT_TYPETAG_STRUCT_NAME);
    DEFINE_INT(JIT_TYPETAG_UNION_NAME);
    DEFINE_INT(JIT_TYPETAG_ENUM_NAME);
    DEFINE_INT(JIT_TYPETAG_CONST);
    DEFINE_INT(JIT_TYPETAG_VOLATILE);
    DEFINE_INT(JIT_TYPETAG_REFERENCE);
    DEFINE_INT(JIT_TYPETAG_OUTPUT);
    DEFINE_INT(JIT_TYPETAG_RESTRICT);
    DEFINE_INT(JIT_TYPETAG_SYS_BOOL);
    DEFINE_INT(JIT_TYPETAG_SYS_CHAR);
    DEFINE_INT(JIT_TYPETAG_SYS_SCHAR);
    DEFINE_INT(JIT_TYPETAG_SYS_UCHAR);
    DEFINE_INT(JIT_TYPETAG_SYS_SHORT);
    DEFINE_INT(JIT_TYPETAG_SYS_USHORT);
    DEFINE_INT(JIT_TYPETAG_SYS_INT);
    DEFINE_INT(JIT_TYPETAG_SYS_UINT);
    DEFINE_INT(JIT_TYPETAG_SYS_LONG);
    DEFINE_INT(JIT_TYPETAG_SYS_ULONG);
    DEFINE_INT(JIT_TYPETAG_SYS_LONGLONG);
    DEFINE_INT(JIT_TYPETAG_SYS_ULONGLONG);
    DEFINE_INT(JIT_TYPETAG_SYS_FLOAT);
    DEFINE_INT(JIT_TYPETAG_SYS_DOUBLE);
    DEFINE_INT(JIT_TYPETAG_SYS_LONGDOUBLE);

    DEFINE_INT(JIT_CALL_NOTHROW);
    DEFINE_INT(JIT_CALL_NORETURN);
    DEFINE_INT(JIT_CALL_TAIL);
}


jit_value_t
jit_value_create_NV_constant(jit_function_t func, const NV value)
{
  jit_constant_t c;
  c.type = jit_type_NV;
  /* Need to use correct type here since conversion to a larger float may,
   * I think, cause conversion & might be lossy? {citation required} */
  /* See src/jit_perl_typemapping.h */
  c.un.LJ_NV_CONST_UNION_MEMBER = (jit_NV)value;
  return jit_value_create_constant(func, &c);
}


jit_value_t
jit_value_create_IV_constant(jit_function_t func, const IV value)
{
  jit_constant_t c;
  c.type = jit_type_IV;
  /* If we just use the largest type here, then later, jit_value_create_constant
   * will use the correct type and value. At least for ints, this must yield the
   * correct number. */
  /* See src/jit_perl_typemapping.h */
  c.un.LJ_IV_CONST_UNION_MEMBER = (jit_IV)value; /* Macro, see src/jit_perl_typemapping.h */
  return jit_value_create_constant(func, &c);
}


jit_value_t
jit_value_create_UV_constant(jit_function_t func, const UV value)
{
  jit_constant_t c;
  c.type = jit_type_UV;
  /* If we just use the largest type here, then later, jit_value_create_constant
   * will use the correct type and value. At least for ints, this must yield the
   * correct number. */
  /* See src/jit_perl_typemapping.h */
  c.un.LJ_UV_CONST_UNION_MEMBER = (jit_UV)value; /* Macro, see src/jit_perl_typemapping.h */
  return jit_value_create_constant(func, &c);
}

