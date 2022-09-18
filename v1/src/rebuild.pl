#!/usr/bin/env perl
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf-8
##################################################################################
# This file is part of the HJCMS Project
#
# Copyright (C) Juergen Heinemann http://www.hjcms.de, (C) 2007-2022
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with this library; see the file COPYING.LIB.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
##################################################################################

use strict;
use warnings;
use POSIX qw(strftime);
use File::Glob qw(:globally :nocase);
use Directory::Iterator;

my @SUB_LIBS;
my @SUB_TARGETS;

sub trim {
  my $_s = shift;
  $_s =~ s/^\s+|\s+$//g;
  $_s =~ s/(\n|\t|\s)+/ /ig;
  return $_s;
};

sub find_sub_project {
  my $f = shift;
  open(FH, '<:encoding(UTF-8)', $f) or die $!;
  while(<FH>){
    my $line = trim($_);
    if($_ =~ m/^SET\b.+(sub|lib|project)(Project|Library)\b/i)
    {
      my ($b) = ($_ =~ m/\"([a-z]+)\"/ig);
      if("$b") {
        push(@SUB_LIBS, $b);
        $f =~ s/^\.(\/)?\b//g;
        $f =~ s/\/CMakeLists\.txt$//g;
        push(@SUB_TARGETS, $f);
      }
    }
  }
  close(FH);
};

sub get_cmakelists {
  my $it = Directory::Iterator->new(".", recursive=>1, show_dotfiles=>0, show_directories=>0);
  while (my $fp = <$it>) {
    if ($fp =~ qr/.+\bCMakeLists.txt$/i) {
      find_sub_project($fp);
    }
  }
};

get_cmakelists();

open(FH, '>:encoding(UTF-8)', "./CMakeSubTargets.cmake") or die $!;
print FH <<EOF;
SET (PROJECT_INCLUDE_DIRS
\t\${PROJECT_INCLUDE_DIRS}
EOF

foreach (@SUB_TARGETS) {
  print FH "\t\${CMAKE_CURRENT_BINARY_DIR}/$_\n";
  print FH "\t\${CMAKE_CURRENT_SOURCE_DIR}/$_\n";
}

print FH <<EOF;
)

INCLUDE_DIRECTORIES (\${PROJECT_INCLUDE_DIRS})

SET(PROJECT_LIBRARIES
EOF

foreach (@SUB_LIBS) {
  print FH "\t$_\n";
}

print FH <<EOF;
)

EOF

close(FH);
