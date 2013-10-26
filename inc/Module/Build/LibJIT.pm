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

sub ACTION_code {
    my ($self) = @_;

    $self->depends_on('detect_types');

    return $self->SUPER::ACTION_code(@_);
}

# Build bootstrap/determine_jit_types.c and run it to generate a
# header file that defines libjit-perl basic type mappings.
sub ACTION_detect_types {
    return;
    my ($self) = @_;

    $self->log_info("Detecting mapping between libjit and Perl types...\n");

    my $file_base = 'determine_jit_types';
    my $cb = $self->cbuilder;
    my $cfile = File::Spec->catfile('bootstrap', "$file_base.c");
    my $obj = $cb->compile(
        source => $cfile,
        include_dirs => [$self->libjit->include_dir],
        #'C++' => 1,
    );
    die "Failed to build $file_base!" if not -f $obj;
    my ($exe, @tmp) = $cb->link_executable(
        objects => $obj,
        extra_linker_flags => [$self->libjit->static_library, '-lpthread'],
    );
    die "Failed to build $file_base!" if not -f $exe;

    my $out = `$exe`;
    die "Failed to run $file_base!" if not defined $out;
    unlink $_ for ($obj, $exe, @tmp);

    open my $fh, ">", File::Spec->catfile("src", "jit_perl_typemapping.h") or die $!;
    print $fh "#ifndef JIT_PERL_TYPEMAPPING_H_\n#define JIT_PERL_TYPEMAPPING_H_\n\n";
    print $fh "/* This file is auto-generated! Edit " . __FILE__ . " instead! */\n\n";
    print $fh "#include <jit/jit.h>\n\n";
    print $fh $out;
    print $fh "\n\n#endif\n";
    close $fh;
}

1;
