package LibJIT::API;

use strict;
use warnings;

use LibJIT;
use LibJIT::API_exports;

use Exporter 'import';

our @AutoFunctions; # from API_exports

my @Functions = (qw(
    jit_context_create
    jit_context_build_start
    jit_context_build_end
    jit_context_destroy

    jit_function_to_closure
    jit_function_apply

    jit_insn_get_native
    jit_insn_call
    jit_insn_call_indirect
    jit_insn_call_native
    jit_insn_label
    jit_insn_branch
    jit_insn_branch_if
    jit_insn_branch_if_not
    jit_insn_address_of_label

    jit_type_create_struct
    jit_type_create_union
    jit_type_create_signature
), @AutoFunctions);

my @Constants = qw(
    jit_abi_cdecl
    jit_abi_vararg
    jit_abi_stdcall
    jit_abi_fastcall

    jit_type_void
    jit_type_sbyte
    jit_type_ubyte
    jit_type_short
    jit_type_ushort
    jit_type_int
    jit_type_uint
    jit_type_nint
    jit_type_nuint
    jit_type_long
    jit_type_ulong
    jit_type_float32
    jit_type_float64
    jit_type_nfloat
    jit_type_void_ptr

    jit_type_sys_bool
    jit_type_sys_char
    jit_type_sys_schar
    jit_type_sys_uchar
    jit_type_sys_short
    jit_type_sys_ushort
    jit_type_sys_int
    jit_type_sys_uint
    jit_type_sys_long
    jit_type_sys_ulong
    jit_type_sys_longlong
    jit_type_sys_ulonglong
    jit_type_sys_float
    jit_type_sys_double
    jit_type_sys_long_double
);

our @EXPORT_OK = (@Functions, @Constants);
our %EXPORT_TAGS = (
    all       => \@EXPORT_OK,
    functions => \@Functions,
    constants => \@Constants,
);

1;
