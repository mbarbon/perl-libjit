#!/usr/bin/env perl

use strict;
use warnings;

system($^X, 'author_tools/make_jitapi_wrappers.pl',
       'src/jitapi.c',
       'src/gen-jitapi.inc', 'src/gen-jitapi.h', 'xsp/gen-jitapi.xsp')
    and die "Error!";

for my $api (qw(function insn type value)) {
    system($^X, 'author_tools/make_libjit_wrappers.pl',
           "libjit/include/jit/jit-${api}.h", "xsp/gen-${api}.xsp")
        and die "Error!";
}
