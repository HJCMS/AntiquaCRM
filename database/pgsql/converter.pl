#!/usr/bin/env perl

=head1 NAME

  migrate.pl - Perl Script to Migrate the HAI Database System

=cut

use strict;
use warnings;
use POSIX qw(strftime);
use Directory::Iterator;
use File::Basename;
use File::Copy;
use MIME::Base64 qw(encode_base64);
use Image::ExifTool;
use Image::Magick;

## Wird für Umlaute auf dem terminial benötigt
# use open ':utf8';
binmode STDOUT, ":utf8";

my $HOMEDIR = $ENV{"HOME"};

my $IMAGE_LOCATION = "$HOMEDIR/Developement/antiqua/database/tmp/BildDaten/2022-05-24";

my $IMAGE_SOURCE_TARGET = "$HOMEDIR/Developement/antiqua/database/tmp/Archiv/Bilder/Import";
my $IMAGE_ZIP_TARGET = "$HOMEDIR/Developement/antiqua/database/tmp/Archiv/Zipped/2022-05-24";

## Valid Values zip | source
my $OPERATION = "zip";

system("mkdir -p $IMAGE_SOURCE_TARGET");

system("mkdir -p $IMAGE_ZIP_TARGET");

sub make_source_archiv {
  my $_file = shift;
  my $_id = shift;
  my $_out  = $IMAGE_SOURCE_TARGET . "/";
  $_out .= sprintf '%08d', $_id;
  $_out .= ".jpg";
  system("rm -f /tmp/out-converter.jpg");
  copy($_file,"/tmp/import-converter.jpg");
  my $exifTool = new Image::ExifTool;
  my $info = $exifTool->ImageInfo("/tmp/import-converter.jpg");
  my $rotate = $exifTool->GetValue('Orientation');
  if($rotate eq 'Horizontal (normal)') {
    $rotate = 90;
  } else {
    $rotate =~ s/\D+//ig;
  }
  print "- Source Archive: $_id $rotate \n";
  ## Strip
  $exifTool->ClearOptions();
  $rotate = (($rotate) && ($rotate >= 90)) ? $rotate : 0;
  ## Convert
  my @args = ("convert","-resize","1028x","-rotate",$rotate,"-quality",95,"-strip","-gamma",0.8,"/tmp/import-converter.jpg","$_out");
  system(@args);
  return;
}

sub make_zip_archiv {
  my $_file = shift;
  my $_id = shift;
  my $_out  = $IMAGE_ZIP_TARGET . "/" . $_id . ".jpg";
  system("rm -f /tmp/out-converter.jpg");
  copy($_file,"/tmp/import-converter.jpg");
  my $exifTool = new Image::ExifTool;
  my $info = $exifTool->ImageInfo("/tmp/import-converter.jpg");
  my $rotate = $exifTool->GetValue('Orientation');
  if($rotate eq 'Horizontal (normal)') {
    $rotate = 90;
  } else {
    $rotate =~ s/\D+//ig;
  }
  print "- ZIP Archive: $_id $rotate \n";
  ## Strip
  $exifTool->ClearOptions();
  $rotate = (($rotate) && ($rotate >= 90)) ? $rotate : 0;
  ## Convert
  my @args = ("convert","-resize","480x","-rotate",$rotate,"-quality",90,"-strip","-gamma",0.7,"/tmp/import-converter.jpg","$_out");
  system(@args);
  return;
}

=begin
  Suche Bilder und rufe Programm auf
=cut
sub createImages {
  my $count = 0;
  my $it = Directory::Iterator->new($IMAGE_LOCATION, show_dotfiles=>0);
  while (my $file = <$it>) {
    $count++;
    my $_id = basename($file);
    if ($_id =~ qr/^\d{1,6}\.JPG$/i) {
      $_id =~ s/\.JPG$//g;
      if($OPERATION eq 'zip') {
        make_zip_archiv($file,$_id);
      } else {
        make_source_archiv($file,$_id);
      }
    } else {
      print "Invalid Format - $count - ID: '$_id'\n";
    }
#     if($count >= 10) {
#       last;
#     }
  }
};

=begin
  Erstelle abfragen
=cut
createImages();

# print "-- script finished\n";
##EOF
