#ifndef _LIBJIT_HELPERS_H
#define _LIBJIT_HELPERS_H

#include <jit/jit.h>

#include "EXTERN.h"
#include <perl.h>

#include <vector>

namespace LibJIT
{
    typedef jit_block_t Block;
    typedef jit_constant_t Constant;
    typedef jit_context_t Context;
    typedef jit_function_t Function;
    typedef jit_insn_t Insn;
    typedef jit_label_t Label;
    typedef jit_type_t Type;
    typedef jit_value_t Value;
    typedef SV * LabelSV; /* Label, verified in typemap, with the
                           * RV stripped, but modifiable since SV* holding IV */
}

jit_value_t jit_value_create_NV_constant(jit_function_t func, const NV value);
jit_value_t jit_value_create_IV_constant(jit_function_t func, const IV value);
jit_value_t jit_value_create_UV_constant(jit_function_t func, const UV value);
jit_value_t jit_value_create_ptr_constant(jit_function_t func, const IV value);
jit_value_t jit_value_create_undef_constant(jit_function_t func);

void lj_define_types(pTHX);
void lj_define_constants(pTHX);
void lj_define_intrinsics(pTHX);

void *lj_convert_sv_to_object(pTHX_ SV *sv, const char *name);

int lj_function_apply(pTHX_ jit_function_t function, SV **sp, int count);

template<typename T>
std::vector<T> lj_convert_avref_to_object_array(pTHX_ SV *sv)
{
    if (!SvROK(sv) || SvTYPE(SvRV(sv)) != SVt_PVAV)
        croak("value is not an array reference");

    AV *av = (AV *)SvRV(sv);
    std::vector<T> res;
    size_t size = av_len(av) + 1;

    res.reserve(size);

    for (int i = 0; i < size; ++i)
    {
        void *obj = lj_convert_sv_to_object(aTHX_ *av_fetch(av, i, 0), "LibJIT::Type");

        res.push_back(static_cast<T>(obj));
    }

    return res;
}

#endif // _LIBJIT_HELPERS_H
