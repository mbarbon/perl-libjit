#!/usr/bin/env perl

use strict;
use warnings;

use Test::More tests => 3;

use LibJIT::API qw(:all);
use Config;

is($Config{ivsize}, jit_type_get_size(jit_type_IV));
is($Config{uvsize}, jit_type_get_size(jit_type_UV));
is($Config{nvsize}, jit_type_get_size(jit_type_NV));
