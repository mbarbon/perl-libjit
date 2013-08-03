#include "helpers.h"

void *lj_convert_sv_to_object(pTHX_ SV *sv, const char *name)
{
    if (sv_isobject(sv) && (SvTYPE(SvRV(sv)) == SVt_PVMG))
        return INT2PTR(void *, SvIV((SV*)SvRV(sv)));
    else
        croak("object is not a blessed scalar reference");
}

std::vector<jit_type_t> lj_convert_avref_to_type_array(pTHX_ SV *sv)
{
    if (!SvROK(sv) || SvTYPE(SvRV(sv)) != SVt_PVAV)
        croak("value is not an array reference");

    AV *av = (AV *)SvRV(sv);
    std::vector<jit_type_t> res;
    size_t size = av_len(av) + 1;

    res.reserve(size);

    for (int i = 0; i < size; ++i)
    {
        void *obj = lj_convert_sv_to_object(aTHX_ *av_fetch(av, i, 0), "LibJIT::Type");

        res.push_back(static_cast<jit_type_t>(obj));
    }

    return res;
}

void lj_from_perl(pTHX_ jit_type_t type, SV *arg, void *buffer)
{
    int kind = jit_type_get_kind(type);

    switch (kind)
    {
    case JIT_TYPE_INT:
        *(jit_int *) buffer = SvIV(arg);
        break;
    default:
        croak("Can't handle signature type");
    }
}

int lj_to_perl(pTHX_ jit_type_t type, void *buffer)
{
    dSP;
    int kind = jit_type_get_kind(type);
    SV *res = 0;

    switch (kind)
    {
    case JIT_TYPE_INT:
        res = newSViv(*(jit_int *) buffer);
        break;
    default:
        croak("Can't handle return type");
    }

    if (res)
        XPUSHs(res);
    PUTBACK;

    return res ? 1 : 0;
}

int lj_function_apply(pTHX_ jit_function_t function, SV **sp, int count)
{
    std::vector<std::vector<char> > args_buffer;
    std::vector<void *> args;
    std::vector<char> result;

    jit_type_t signature = jit_function_get_signature(function);
    int size = jit_type_num_params(signature);
    jit_type_t ret = jit_type_get_return(signature);

    result.resize(jit_type_get_size(ret));

    args_buffer.resize(size);
    args.resize(size);

    for (int i = 0; i < size; ++i)
    {
        jit_type_t param = jit_type_get_param(signature, i);

        args_buffer[i].resize(jit_type_get_size(param));
        lj_from_perl(aTHX_ param, sp[i], &args_buffer[i][0]);
        args[i] = &args_buffer[i][0];
    }

    jit_function_apply(function, &args[0], &result[0]);

    return lj_to_perl(aTHX_ ret, &result[0]);
}
