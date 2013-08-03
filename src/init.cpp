#include "helpers.h"

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

void lj_define_types(pTHX)
{
    DEFINE_TYPE(int);
}

void lj_define_constants(pTHX)
{
    DEFINE_INT(jit_abi_cdecl);
    DEFINE_INT(jit_abi_vararg);
    DEFINE_INT(jit_abi_stdcall);
    DEFINE_INT(jit_abi_fastcall);
}
