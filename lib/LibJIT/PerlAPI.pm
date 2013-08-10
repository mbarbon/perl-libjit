package LibJIT::PerlAPI;

use strict;
use warnings;

use LibJIT;
use LibJIT::PerlAPI_exports;

use Exporter 'import';

our @AutoFunctions; # from API_exports

my @Functions = (qw(
), @AutoFunctions);

our @EXPORT_OK = (@Functions);
our %EXPORT_TAGS = (
    all       => \@EXPORT_OK,
    functions => \@Functions,
);

1;
