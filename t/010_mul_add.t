#!/usr/bin/env perl

use strict;
use warnings;

use Test::More tests => 2;

use LibJIT;

my $cxt = LibJIT::API::jit_context_create();

LibJIT::API::jit_context_build_start($cxt);

my $signature = LibJIT::API::jit_type_create_signature(
    LibJIT::API::jit_abi_cdecl, LibJIT::API::jit_type_int,
    [LibJIT::API::jit_type_int, LibJIT::API::jit_type_int, LibJIT::API::jit_type_int], 1);
my $fun = LibJIT::API::jit_function_create($cxt, $signature);

# emit code
my ($x, $y, $z) = map LibJIT::API::jit_value_get_param($fun, $_), 0..2;
my $t1 = LibJIT::API::jit_insn_mul($fun, $x, $y);
my $t2 = LibJIT::API::jit_insn_add($fun, $t1, $z);
LibJIT::API::jit_insn_return($fun, $t2);

LibJIT::API::jit_function_compile($fun);
LibJIT::API::jit_context_build_end($cxt);

my $r1 = LibJIT::API::jit_function_apply($fun, 3, 5, 2);
is($r1, 17);

my $r2 = LibJIT::API::jit_function_apply($fun, 5, 8, 2);
is($r2, 42);

LibJIT::API::jit_context_destroy($cxt);
