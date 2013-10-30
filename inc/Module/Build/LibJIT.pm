package Module::Build::LibJIT;

use warnings;
use strict;

use parent 'Module::Build::WithXSpp';

use Alien::LibJIT;
use FindBin('$Bin');
use File::Spec;
use Config;
use ExtUtils::CBuilder;

__PACKAGE__->add_property( 'libjit' );

sub new {
    my ($class, @args) = @_;

    my $alien = Alien::LibJIT->new;
    my $self = $class->SUPER::new(
        libjit             => $alien,
        include_dirs       => [$alien->include_dir],
        extra_linker_flags => [$alien->static_library, '-lpthread'],
        @args,
    );
    return $self;
}

1;
