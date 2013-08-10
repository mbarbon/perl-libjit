#!/usr/bin/env perl

use strict;
use warnings;

my @headers = map "libjit/include/jit/jit-${_}.h", qw(function insn type value);

system($^X, 'author_tools/make_jitapi_wrappers.pl',
       'src/jitapi.c',
       'src/gen-jitapi.inc', 'src/gen-jitapi.h', 'xsp/gen-jitapi.xsp',
       'lib/LibJIT/PerlAPI_exports.pm')
    and die "Error!";

system($^X, 'author_tools/make_libjit_wrappers.pl',
       "xsp/gen-api.xsp", 'lib/LibJIT/API_exports.pm', @headers)
    and die "Error!";
