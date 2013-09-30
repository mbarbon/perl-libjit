#include <stdio.h>
#include <jit/jit.h>
#include <EXTERN.h>
#include <perl.h>

int
main(int argc, char **argv)
{
#define TEST_TYPE(Size, Jit_type, Ctype, Union_member)   \
    if (Size == (size_t)jit_type_get_size((Jit_type))) { \
        c_type = (char *)#Ctype;                         \
        jit_type = (char *)#Jit_type;                    \
        union_member = (char *)Union_member;             \
    }

    const size_t iv_size = sizeof(IV);
    const size_t nv_size = sizeof(NV);
    const size_t uv_size = sizeof(UV);
    char *c_type;
    char *jit_type;
    char *union_member;
    char *big_IV_union_mem;
    char *big_UV_union_mem;


    TEST_TYPE(nv_size, jit_type_nfloat, jit_nfloat, "nfloat_value")
    else TEST_TYPE(nv_size, jit_type_float64, jit_float64, "float64_value")
    else TEST_TYPE(nv_size, jit_type_float32, jit_float32, "float32_value")
    else TEST_TYPE(nv_size, jit_type_sys_float, float, "nfloat_value") /* no clear mapping, default to biggest in union */
    else TEST_TYPE(nv_size, jit_type_sys_double, double, "nfloat_value") /* no clear mapping, default to biggest in union */
    else TEST_TYPE(nv_size, jit_type_sys_long_double, long double, "nfloat_value") /* no clear mapping, default to biggest in union */
    else {
        printf("Failed to find JIT floating point type that has the same size as Perl's NVs\n");
        abort();
    }
    printf("typedef %s jit_NV;\n", c_type);
    printf("#define jit_type_NV %s\n", jit_type);
    printf("#define LJ_NV_CONST_UNION_MEMBER %s\n\n", union_member);


    if (sizeof(jit_nint) > sizeof(jit_int)) {
      if (sizeof(jit_nint) > sizeof(jit_long))
        big_IV_union_mem = "nint_value";
      else
        big_IV_union_mem = "long_value";
    }
    else {
      if (sizeof(jit_int) > sizeof(jit_long))
        big_IV_union_mem = "int_value";
      else
        big_IV_union_mem = "long_value";
    }
    TEST_TYPE(iv_size, jit_type_nint, jit_nint, "nint_value")
    else TEST_TYPE(iv_size, jit_type_int, jit_int, "int_value")
    else TEST_TYPE(iv_size, jit_type_long, jit_long, "long_value")
    else TEST_TYPE(iv_size, jit_type_short, jit_short, "int_value")
    else TEST_TYPE(iv_size, jit_type_sys_int, int, big_IV_union_mem)
    else TEST_TYPE(iv_size, jit_type_sys_long, long, big_IV_union_mem)
    else TEST_TYPE(iv_size, jit_type_sys_longlong, long long, big_IV_union_mem)
    else TEST_TYPE(iv_size, jit_type_sys_short, short, "jit_int")
    else {
        printf("Failed to find JIT integer type that has the same size as Perl's IVs\n");
        abort();
    }
    printf("typedef %s jit_IV;\n", c_type);
    printf("#define jit_type_IV %s\n", jit_type);
    printf("#define LJ_IV_CONST_UNION_MEMBER %s\n\n", union_member);


    if (sizeof(jit_nuint) > sizeof(jit_uint)) {
      if (sizeof(jit_nuint) > sizeof(jit_ulong))
        big_UV_union_mem = "nuint_value";
      else
        big_UV_union_mem = "ulong_value";
    }
    else {
      if (sizeof(jit_uint) > sizeof(jit_ulong))
        big_UV_union_mem = "uint_value";
      else
        big_UV_union_mem = "ulong_value";
    }
    TEST_TYPE(uv_size, jit_type_nuint, jit_nuint, "nuint_value")
    else TEST_TYPE(uv_size, jit_type_uint, jit_uint, "uint_value")
    else TEST_TYPE(uv_size, jit_type_ulong, jit_ulong, "ulong_value")
    else TEST_TYPE(uv_size, jit_type_sys_uint, unsigned int, big_UV_union_mem)
    else TEST_TYPE(uv_size, jit_type_sys_ulong, unsigned long, big_UV_union_mem)
    else TEST_TYPE(uv_size, jit_type_sys_ulonglong, unsigned long long, big_UV_union_mem)
    else TEST_TYPE(uv_size, jit_type_sys_ushort, unsigned short, "jit_uint")
    else {
        printf("Failed to find JIT unsigned integer type that has the same size as Perl's UVs\n");
        abort();
    }
    printf("typedef %s jit_UV;\n", c_type);
    printf("#define jit_type_UV %s\n", jit_type);
    printf("#define LJ_UV_CONST_UNION_MEMBER %s\n\n", union_member);
}
