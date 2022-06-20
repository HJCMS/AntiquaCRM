#!/usr/bin/env perl

=head1 NAME

  migrate.pl - Perl Script to Migrate the HAI Database System

=cut

use strict;
use warnings;
use POSIX qw(strftime);
use DBI;
use Directory::Iterator;
use File::Basename;
use File::Copy;
use MIME::Base64 qw(encode_base64);
use Image::ExifTool;
use Image::Magick;

## Wird für Umlaute auf dem terminial benötigt
# use open ':utf8';
binmode STDOUT, ":utf8";

my $MAX_SIZE = 320;

my $HOMEDIR = $ENV{"HOME"};

my $IMAGE_LOCATION = "$HOMEDIR/Developement/antiqua/database/tmp/Archiv/Bilder";

=begin
  Schreibe die Tabelle in Datei für Script psql-migration.sh
=cut
sub init_image_file {
  open(FH, '>:encoding(UTF-8)', "./public.inventory_images.sql") or die $!;
    print FH "TRUNCATE inventory_images;\n";
  close(FH);
};

sub add_image_file {
  my $_t = shift;
  if(length($_t)>4) {
    open(FH, '>>:encoding(UTF-8)', "./public.inventory_images.sql") or die $!;
    print FH $_t ."\n";
    close(FH);
  }
};

sub image_resize_encode_base64 {
  my $_f = shift;
  ## Convert
  system("rm -f /tmp/out.jpg");
  my @args = ("convert","-resize","${MAX_SIZE}x","-quality",85,"$_f","/tmp/out.jpg");
  system(@args);
  ## base64 convert
  if (-s "/tmp/out.jpg")
  {
    my $buf;
    my $out;
    open(FILE, "/tmp/out.jpg") or die "$!";
    while (read(FILE, $buf, 60*57)) {
      $out .= encode_base64($buf);
    }
    return $out;
  }
  return;
}

=begin
  Suche Bilder
=cut
sub createImageIndex {
  my $count = 0;
  init_image_file();
  my $it = Directory::Iterator->new($IMAGE_LOCATION, show_dotfiles=>0);
  while (my $file = <$it>) {
    $count++;
    my $_id = basename($file);
    if ($_id =~ qr/^\d{1,8}\.jpg$/i) {
      my $_base64 = image_resize_encode_base64($file);
      if($_base64) {
        my $sql = "INSERT INTO inventory_images (im_id,im_imgdata,im_changed) VALUES ";
        $_id =~ s/\D+//ig;
        $_base64 =~ s/[\n\r]+//ig;
        $sql .= "($_id,'$_base64',CURRENT_TIMESTAMP);\n";
        add_image_file($sql);
      }
    } else {
      print "Invalid Format - $count - ID: '$_id'\n";
    }
  }
};

=begin
  Erstelle abfragen
=cut
createImageIndex();

# print "-- script finished\n";
##EOF
