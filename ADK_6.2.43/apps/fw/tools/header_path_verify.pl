#!/usr/bin/perl -w
# Copyright (c) 2016 Qualcomm Technologies International, Ltd.
#   %%version

# This file looks for various problems with headers:
#
# - Headers that don't live in an include directory but are included
#   without specifying the path.  For this purpose, include directories
#   are those beginning with "core/include", plus the hardware-specific
#   subdirectories of hal and io.
#
# - Duplicate names of headers other than those in the include directories,
#   where duplication is allowed to resolve OS, hardware and firmware
#   vs. host variations.
#
# make_all.sh already checks for an explicit "include/", otherwise
# this would do that.
#
# Feel free to rewrite it in Python if you happen to know how to
# do "find" in Python.

use File::Find;
use Data::Dumper;

# Find header files in include directories.
# Also find header files in hal and IO per-chip directories.
our %includes;

my $ret = 0; # Accumulating return code
my $error_count = 0; # For reporting

# Files exempted from the general "unique file names" rule and the directories
# they are alowed to live in. There should be a very good reason to use this!!!
@exemptions = (
    ["hal_macros_divert.h", "hal", "include"],
    ["main.h", "core", "customer"]);
#print Dumper(@exemptions);


foreach my $file (glob("core/include*/*"), glob("core/hal/*/*/hal/*"),
                  glob("core/io/*/*/io/*")) {
    if (-f $file) {
	my $base = $file;
	$base =~ s%^.*/%%;
	$includes{$base} = 1;
    }
}

sub wanted_includes {
    return if $_ eq ".";
    if ($File::Find::dir !~ m%(^|/)include% or
       (-d _ && !m%(^|/)include%)) {
	$File::Find::prune = 1;
	return;
    }

    $includes{$_} = 1  if -f _;
}

find(\&wanted_includes, ".");

# Search through files to see if they include files without a
# / other than those we just found.
#
# Do this recursively --- finds hal and IO stuff.

our %seen;

sub exempt {
    # Some headers are allowed in more than one place.

    $exempt_files_no = $#exemptions + 1;

    $file = shift @_;
    @dirs = @_;
    $dirs_no = $#dirs + 1;

    for ($i = 0; $i < $exempt_files_no; $i++){
        @exemption = @{ $exemptions[$i] };
        #print "@exemption\n";

        $exempt_file = shift @exemption;
        @exempt_dirs = @exemption;

        if ($file eq $exempt_file){
            $missing = 0;
            for ($j = 0; $j < $dirs_no; $j++){
                $dir = $dirs[$j];
                if (not $dir ~~ @exempt_dirs){
                    $missing = 1;
                }
            }
            if (not $missing){
                # File is allowed in the given dirs.
                return 1;
            }
        }
    }

    #File is NOT allowed in the given dirs.
    return 0;
}

sub wanted {
    # $File::Find::dir is the current directory name,
    # $_ is the current filename within that directory
    # $File::Find::name is the complete pathname to the file.
    return unless /\.[ch]$/;

    # Check for header duplicates.
    if (/\.h$/) {
        #print "Considering $_\n";
	if ($seen{$_}) {
	    # Allow duplication within include directories, including
	    # hardware-specific
            # Again, we need the same top_dir stuff
            $top_dir = $File::Find::dir;
            # Get rid of the starting ./
            $top_dir =~ s?./??;
            # Get rid os subdirectories
            $top_dir =~ s?/.*$??;
            # Treat all include directories the same
            $top_dir =~ s?include_.*?include?;
            #print "Comparing $top_dir with $seen{$_}\n";
	    if ($top_dir ne $seen{$_}) {
                if (not exempt($_, $top_dir, $seen{$_})) {
                    print "WARNING: found $_ in both $seen{$_} ",
                    "and $File::Find::dir\n";
                    $ret = 1;
                }
	    }
	} else {
            $top_dir = $File::Find::dir;
            # Get rid of the starting ./
            $top_dir =~ s?./??;
            # Get rid os subdirectories
            $top_dir =~ s?/.*$??;
            # Treat all include directories the same
            $top_dir =~ s?include_.*?include?;
	    $seen{$_} = $top_dir;
            #print "Adding $File: //depot/bg/Releases/ADK/ADK6.2/hydra_os_aura/fw/tools/header_path_verify.pl $_ and dir $top_dir, giving $seen{$_}\n";
	}
    }

    if ($File::Find::dir !~ /^\.\/wlan\/(ip|include)/) {
        if (!open FILE, $_) {
        warn "Can't read $File::Find::name";
        return;
        }

        # Check for headers without paths
        $lineno = 0;
        foreach $line (<FILE>) {
            $lineno++;
        if ($line =~ m%^\s*\#\s*include\s+\"([^\"/]+)\"%) {
            if (!$includes{$1}) {
            print "${File::Find::name}:$lineno: WARNING: header $1:\n",
                        "    not in an include directory, included without path\n";
                    $ret = 1;
                    $error_count += 1;
            }
        }
        }

        close FILE;
    }
}

find(\&wanted, ".");
#print "\nScript $0 found $error_count problems\n";
exit $ret;
