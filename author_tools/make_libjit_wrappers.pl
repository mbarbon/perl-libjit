#!/usr/bin/env perl

use strict;
use warnings;

use autodie qw(open);

my @IGNORE_FUNCTIONS = map { s/\s#.*$//; $_ } split /\n/, <<EOT;
jit_constant_convert # TODO
jit_function_apply
jit_function_apply_vararg # TODO
jit_function_get_meta # TODO
jit_function_set_meta # TODO
jit_function_free_meta # TODO
jit_function_compile_entry
jit_function_setup_entry
jit_function_from_closure # TODO?
jit_function_from_pc # TODO?
jit_function_from_vtable_pointer # TODO?
jit_function_to_closure
jit_function_to_vtable_pointer # TODO
jit_function_get_on_demand_compiler # TODO
jit_function_set_on_demand_compiler # TODO
jit_insn_get_native
jit_insn_call_native
jit_insn_call_intrinsic # TODO
jit_insn_label
jit_insn_branch
jit_insn_branch_if
jit_insn_branch_if_not
jit_insn_jump_table # TODO
jit_insn_call
jit_insn_call_indirect
jit_insn_call_indirect_vtable # TODO
jit_insn_address_of_label
jit_insn_branch_if_pc_not_in_range # TODO
jit_insn_start_finally # TODO
jit_insn_call_finally # TODO
jit_insn_start_filter # TODO
jit_insn_call_filter # TODO
jit_insn_iter_init # TODO?
jit_insn_iter_init_last # TODO?
jit_insn_iter_next # TODO?
jit_insn_iter_previous # TODO?
jit_type_create_struct
jit_type_create_union
jit_type_create_signature
jit_type_create_tagged # TODO
jit_type_set_names # TODO
jit_type_get_tagged_data # TODO
jit_type_set_tagged_data # TODO
jit_value_create_constant # TODO
jit_value_get_constant # TODO
EOT
my $MAYBE_TYPES = '(?:jit_\w+|void|int|long|unsigned|char)';

my ($xsp, $pm, @headers) = @ARGV;

unless ($xsp && $pm && @headers) {
    print STDERR <<EOH;
Usage: $0 <output XSP file> <output PM file> <input headers>
EOH
    exit 1;
}

sub process_function {
    my ($text) = @_;

    $text =~ tr{\n\t }{   }s;
    $text =~ s{\s+(?:$|(?=\())}{}g;

    die "Unable to extract function name from '$text'"
        unless $text =~ m{^\S.+?(jit_\w+)};
    my $function = $1;

    return ('', "# ignoring $function\n")
        if grep $_ eq $function, @IGNORE_FUNCTIONS;

    $text =~ s{jit_(?!abi)(\w+)_t\b}{LibJIT::\u$1}g;
    $text =~ s{\s+JIT_NOTHROW$}{};

    return ("    $function\n", "$text;\n");
}

sub process_file {
    my ($input, $ofhx, $ofhp) = @_;
    my $text;

    {
        local $/;
        open my $ifh, '<', $input;
        $text = <$ifh>;
    }

    while ($text =~ m{^($MAYBE_TYPES\b.*?);$}gms) {
        my ($pm, $xsp) = process_function($1);

        print $ofhx $xsp;
        print $ofhp $pm;
    }
}

sub process_files {
    my ($headers, $xs, $pm) = @_;

    open my $ofhx, '>', $xs;
    open my $ofhp, '>', $pm;

    print $ofhx <<EOT;
# THIS FILE IS GENERATED BY author_tools/make_libjit_wrappers.pl
# DO NOT EDIT

%module{LibJIT};

%package{LibJIT::API};

#include "helpers.h"

EOT

    print $ofhp <<EOT;
# THIS FILE IS GENERATED BY author_tools/make_libjit_wrappers.pl
# DO NOT EDIT

package LibJIT::API;

our \@AutoFunctions = qw(
EOT

    for my $header (@$headers) {
        process_file($header, $ofhx, $ofhp);
    }

    print $ofhp <<EOT;
);

1;
EOT
}

process_files(\@headers, $xsp, $pm);
