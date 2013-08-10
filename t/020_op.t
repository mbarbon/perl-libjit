#!/usr/bin/env perl

use strict;
use warnings;

use Test::More tests => 2;

use LibJIT::API qw(:all);
use LibJIT::PerlAPI qw(:all);
use B::Replace;
use B::Generate;
use B;

sub inc {
    my ($a) = @_;

    return $a + 1;
}

# find add op
my $inc = B::svref_2object(\&inc);
my $add = $inc->START;

$add = $add->next until $add->name eq 'add';

# generate new OP
my $cxt = jit_context_create();

jit_context_build_start($cxt);

# TODO actually dispatch the optree...
my $signature = jit_type_create_signature(
    jit_abi_cdecl, jit_type_void_ptr, [jit_type_void_ptr], 1);
my $fun = jit_function_create($cxt, $signature);
my $thx = jit_value_get_param($fun, 0);

# padsv, no lval_intro or deref
my $padix = jit_value_create_nint_constant($fun, jit_type_nint, $add->first->targ);
my $t1 = pa_get_pad_sv($fun, $thx, $padix);
my $tleft = pa_sv_2nv($fun, $thx, $t1);

# float const, in pad if threaded
my $constsv = ${$add->last->sv} ? ${$add->last->sv} : $inc->PADLIST->ARRAYelt(1)->ARRAYelt($add->last->targ);
my $tright = jit_value_create_float64_constant($fun, jit_type_float64, ${$constsv->object_2svref});

# addition
my $tres = jit_insn_add($fun, $tleft, $tright);

# set targ and push
my $targ = pa_get_targ($fun, $thx);
pa_sv_set_nv($fun, $thx, $targ, $tres);
pa_push_sv($fun, $thx, $targ);
jit_insn_return($fun, pa_get_op_next($fun, $thx));

jit_function_compile($fun);
jit_context_build_end($cxt);

my $replacement = B::OP->new('null', 0);
$replacement->ppaddr(jit_function_to_closure($fun));
$replacement->targ($add->targ);
$replacement->next($add->next);

# swap the new op in place
B::Replace::replace_tree($inc->ROOT, $add, $replacement);

is(inc(2), 3);
is(inc(3.2), 4.2);
