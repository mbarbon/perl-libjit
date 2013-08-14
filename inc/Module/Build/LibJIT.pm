package Module::Build::LibJIT;

use warnings;
use strict;

use parent 'Module::Build::WithXSpp';

use FindBin('$Bin');
use File::Spec;
use Config;
use ExtUtils::CBuilder;

our $LIBJIT_HOME = 'libjit';
our $LIBJIT_M4 = 'm4';
our $LIBJIT_INCLUDE = File::Spec->catfile($LIBJIT_HOME, 'include');
our $LIBJIT_RESULT = File::Spec->catfile($LIBJIT_HOME, 'jit', '.libs', 'libjit'.$Config::Config{lib_ext});

sub new {
    my ($class, @args) = @_;

    return $class->SUPER::new(
        include_dirs       => [$LIBJIT_INCLUDE],
        extra_linker_flags => [$LIBJIT_RESULT, '-lpthread'],
        @args,
    );
}

sub ACTION_code {
    my ($self) = @_;

    $self->depends_on('libjit');
    $self->depends_on('detect_types');

    return $self->SUPER::ACTION_code(@_);
}

# Build bootstrap/determine_jit_types.c and run it to generate a
# header file that defines libjit-perl basic type mappings.
sub ACTION_detect_types {
    my ($self) = @_;

    $self->depends_on('libjit');

    $self->log_info("Detecting mapping between libjit and Perl types...\n");

    my $file_base = 'determine_jit_types';
    my $cb = $self->cbuilder;
    my $cfile = File::Spec->catfile('bootstrap', "$file_base.c");
    my $obj = $cb->compile(
        source => $cfile,
        include_dirs => [$LIBJIT_INCLUDE],
        #'C++' => 1,
    );
    die "Failed to build $file_base!" if not -f $obj;
    my ($exe, @tmp) = $cb->link_executable(
        objects => $obj,
        extra_linker_flags => [$LIBJIT_RESULT, '-lpthread'],
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


sub ACTION_depcheck {
    my ($self) = @_;
    foreach my $cmd (qw/autoreconf libtool flex bison/) {
        $self->log_info("Checking if '$cmd' is available\n");
        system("$cmd --help > /dev/null 2>&1")
            and die "You need to make sure you have a recent '$cmd' installed and " .
                'that it can be found in your PATH';
    }
    return 1;
}

sub ACTION_libjit {
    my ($self) = @_;

    if(-f $LIBJIT_RESULT) {
        $self->log_info("libjit already built\n");
        return 1;
    }

    $self->depends_on('depcheck');

    my $orig = Cwd::cwd();

    $self->log_info("Changing directories to build libjit\n");
    chdir($LIBJIT_HOME) or die "Failed to cd into '$LIBJIT_HOME'";

    $self->log_info("Creating '$LIBJIT_M4' directory for autoreconf\n");
    -d $LIBJIT_M4 or mkdir($LIBJIT_M4) or die "Failed to mkdir '$LIBJIT_M4'";

    $self->log_info("Running autoreconf\n");
    system('autoreconf', '-i', '-f')
        and die "Failed to run autoreconf";

    $self->log_info("Running ./configure\n");
    #system('./configure', '-enable-shared=false')
    $ENV{CFLAGS} .= " -fPIC";
    system('./configure')
        and die "Failed to configure libjit!";

    $self->log_info("Running make\n");
    system('make') and die "Failed to build libjit!";

    $self->log_info("Returning to our original directory\n");

    chdir($orig);

    if(-f $LIBJIT_RESULT) {
        $self->log_info("Built libjit successfully\n");
        return 1;
    }
    else {
        die "We built libjit, but the lib isn't where I wanted it: $LIBJIT_RESULT";
    }
}

1;
