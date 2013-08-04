package LibJIT::API;

use strict;
use warnings;

use LibJIT;

use Exporter 'import';

my @Functions = qw(
    jit_context_create
    jit_context_build_start
    jit_context_build_end
    jit_context_destroy

    jit_function_create
    jit_function_compile
    jit_function_apply

    jit_insn_add
    jit_insn_mul
    jit_insn_return

    jit_type_create_signature

    jit_value_get_param
);

my @Constants = qw(
    jit_abi_cdecl
    jit_abi_vararg
    jit_abi_stdcall
    jit_abi_fastcall

    jit_type_int
);

our @EXPORT_OK = (@Functions, @Constants);
our %EXPORT_TAGS = (
    all       => \@EXPORT_OK,
    functions => \@Functions,
    constants => \@Constants,
);

1;
