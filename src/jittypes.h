#ifndef _LIBJIT_TYPES_H
#define _LIBJIT_TYPES_H

#include <jit/jit.h>

// if-then-else template
template<bool C, class T, class E>
struct JitIf
{
  typedef T type;
};

template<class T, class E>
struct JitIf<false, T, E>
{
  typedef E type;
};

// check type size
class T_Fail { };

template<class CT, class T1 = T_Fail, class T2 = T_Fail, class T3 = T_Fail, class T4 = T_Fail>
struct JitCheck
{
  typedef typename JitIf<sizeof(CT) == sizeof(typename T1::type), T1,
    typename JitCheck<CT, T2, T3, T4>::type>::type type;
};

template<class CT>
struct JitCheck<CT, T_Fail, T_Fail, T_Fail, T_Fail>
{
  typedef T_Fail type;
};

#define JIT_MAP(_name, _type, _jit_type, _union) \
  struct _name \
  { \
    typedef _type type; \
    static inline jit_type_t jit_type() { return _jit_type; } \
    static void make_constant(jit_constant_t &c, _type value)  \
    { \
      c.type = _jit_type; \
      c.un._union = value; \
    } \
  }

// since _sys_ types are just tagged aliases for the non-sys types,
// there is no need to define a struct for them
JIT_MAP(T_Float, jit_float32, jit_type_float32, float32_value);
JIT_MAP(T_Double, jit_float64, jit_type_float64, float64_value);
JIT_MAP(T_NDouble, jit_nfloat, jit_type_nfloat, nfloat_value);

JIT_MAP(T_Byte, jit_sbyte, jit_type_sbyte, int_value);
JIT_MAP(T_UByte, jit_ubyte, jit_type_ubyte, uint_value);
JIT_MAP(T_Short, jit_short, jit_type_short, int_value);
JIT_MAP(T_UShort, jit_ushort, jit_type_ushort, uint_value);
JIT_MAP(T_Int, jit_int, jit_type_int, int_value);
JIT_MAP(T_UInt, jit_uint, jit_type_uint, uint_value);
JIT_MAP(T_NInt, jit_nint, jit_type_nint, nint_value);
JIT_MAP(T_NUInt, jit_nuint, jit_type_nuint, nuint_value);
JIT_MAP(T_Long, jit_long, jit_type_long, long_value);
JIT_MAP(T_ULong, jit_ulong, jit_type_ulong, ulong_value);

// and finally the actual mapping
typedef JitCheck<NV, T_Float, T_Double, T_NDouble>::type JitTypemap_NV;
typedef JitCheck<IV, T_Int, T_NInt, T_Long>::type JitTypemap_IV;
typedef JitCheck<UV, T_UInt, T_NUInt, T_ULong>::type JitTypemap_UV;

typedef JitTypemap_NV::type jit_NV;
#define jit_type_NV JitTypemap_NV::jit_type()
#define make_NV_constant(c, value) JitTypemap_NV::make_constant(c, value)

typedef JitTypemap_IV::type jit_IV;
#define jit_type_IV JitTypemap_IV::jit_type()
#define make_IV_constant(c, value) JitTypemap_IV::make_constant(c, value)

typedef JitTypemap_UV::type jit_UV;
#define jit_type_UV JitTypemap_UV::jit_type()
#define make_UV_constant(c, value) JitTypemap_UV::make_constant(c, value)

#endif // _LIBJIT_TYPES_H
