#!/usr/bin/env perl

use strict;
use warnings;

use autodie qw(open);

my ($input, $output_c, $output_h, $output_xsp, $output_pm) = @ARGV;

unless ($input && $output_c && $output_h && $output_xsp && $output_pm) {
    print STDERR <<EOH;
Usage: $0 <input C file> <output C file> <output H file> <output XSP file> <output PM file>
EOH
    exit 1;
}

sub jit_type {
    my ($text) = @_;

    $text =~ s/^\s+|\s+$//g;
    $text =~ s/\s+\w+\s*$//;

    return "jit_type_$1" if $text =~ /^(?:jit_)?([IUN]V)/;
    return 'jit_type_void_ptr' if $text =~ /\*/;
    return "jit_type_$1" if $text =~ /^jit_(\w+)$/;
    return 'jit_type_void' if $text eq 'void';

    die $text;
}

sub arg_name {
    my ($text) = @_;

    $text =~ s/\b(\w+)\s*$// or die "Unable to get argument name from '$text'";

    return $1;
}

sub process_function {
    my ($text) = @_;

    $text =~ m{^static\s+(.+)_(pa_\w+)\(pTHX_?(?:\s*(.*))?\)$} or die "Unable to parse '$text'";

    my ($ret, $func, $args) = ($1, $2, $3);
    my @args = split /\s*,\s*/, $args;

    my $jit_ret_type = jit_type($ret);
    my @jit_arg_types = map jit_type($_), @args;
    my @jit_arg_names = map arg_name($_), @args;
    my $jitparms = @jit_arg_names ? join ', ', ('', map "jit_value_t $_", @jit_arg_names) : '';
    my $jittypes = @jit_arg_types ? join ', ', ('', @jit_arg_types) : '';
    my $jitargs = @jit_arg_names ? join ', ', ('', @jit_arg_names) : '';

    my $c = <<EOT;
static jit_type_t _${func}_parms[] = {jit_tTHX${jittypes}};
static jit_type_t _${func}_sig = jit_type_create_signature(jit_abi_cdecl, $jit_ret_type, _${func}_parms, SIZE(_${func}_parms), 1);

jit_value_t ${func}(jit_function_t function${jitparms})
{
    jit_value_t _args[] = {jit_aTHX$jitargs};
    return jit_insn_call_native(function, "$func", (void *)_${func}, _${func}_sig, _args, SIZE(_args), 0);
}

EOT

    my $h = sprintf <<EOT;
jit_value_t ${func}(jit_function_t function${jitparms});
EOT
    (my $xsp = $h) =~ s{jit_(\w+)_t}{LibJIT::\u$1}g;
    my $pm = "    ${func}\n";

    return ($c, $h, $xsp, $pm);
}

sub process_file {
    my ($input, $output_c, $output_h, $output_xsp, $output_pm) = @_;
    my $text;

    {
        local $/;
        open my $ifh, '<', $input;
        $text = <$ifh>;
    }
    open my $ofhc, '>', $output_c;
    open my $ofhh, '>', $output_h;
    open my $ofhx, '>', $output_xsp;
    open my $ofhp, '>', $output_pm;

    print $ofhc <<EOT;
/* THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl */
/* DO NOT EDIT */

#define SIZE(a) (sizeof(a) / sizeof(a[0]))

EOT

    print $ofhh <<EOT;
/* THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl */
/* DO NOT EDIT */

#ifndef _GEN_LIBJIT_PERLAPI_H
#define _GEN_LIBJIT_PERLAPI_H

#include <jit/jit.h>

#ifdef PERL_IMPLICIT_CONTEXT
#define jit_gTHX  ((jit_value_t) jit_function_get_meta(function, LIBJIT_THX_TYPE))
#define jit_tTHX  jit_type_void_ptr
#define jit_aTHX  jit_gTHX
#else
#define jit_tTHX
#define jit_aTHX
#endif

EOT

    print $ofhx <<EOT;
# THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl
# DO NOT EDIT

%module{LibJIT};

%package{LibJIT::PerlAPI};

#include "helpers.h"
#include "jitapi.h"

EOT

    print $ofhp <<EOT;
# THIS FILE IS GENERATED BY author_tools/make_jitapi_wrappers.pl
# DO NOT EDIT

package LibJIT::PerlAPI;

our \@AutoFunctions = qw(
EOT

    while ($text =~ m{^(static.*\(.*\))$}mg) {
        my ($c, $h, $xsp, $pm) = process_function($1);

        print $ofhc $c;
        print $ofhh $h;
        print $ofhx $xsp;
        print $ofhp $pm;
    }

    print $ofhh <<EOT;

#endif // _GEN_LIBJIT_PERLAPI_H
EOT

    print $ofhp <<EOT;
);

1;
EOT
}

process_file($input, $output_c, $output_h, $output_xsp, $output_pm);
