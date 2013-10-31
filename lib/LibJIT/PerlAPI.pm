package LibJIT::PerlAPI;

use strict;
use warnings;

use LibJIT;
use LibJIT::PerlAPI_exports;

use Exporter 'import';

our @AutoFunctions; # from API_exports

my @Functions = (qw(
    pa_create_pp
    pa_pp_op
    pa_pp_nextstate
    pa_pp_unstack
), @AutoFunctions);

our @EXPORT_OK = (@Functions);
our %EXPORT_TAGS = (
    all       => \@EXPORT_OK,
    functions => \@Functions,
);

1;
