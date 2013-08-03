#ifndef _LIBJIT_HELPERS_H
#define _LIBJIT_HELPERS_H

#include <jit/jit.h>

#include "EXTERN.h"
#include <perl.h>

#include <vector>

namespace LibJIT
{
    typedef jit_context_t Context;
    typedef jit_type_t Type;
    typedef jit_function_t Function;
    typedef jit_value_t Value;
}

void lj_define_types(pTHX);
void lj_define_constants(pTHX);

void *lj_convert_sv_to_object(pTHX_ SV *sv, const char *name);

std::vector<jit_type_t> lj_convert_avref_to_type_array(pTHX_ SV *sv);

int lj_function_apply(pTHX_ jit_function_t function, SV **sp, int count);

#endif // _LIBJIT_HELPERS_H
