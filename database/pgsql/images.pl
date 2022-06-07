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

# rpm -qa perl-* | grep -ie DBD-Pg -ie DBD-mysql
# man 3 DBD::mysql
# https://metacpan.org/pod/DBD::mysql
my $dsn = "DBI:mysql:database=hai;host=localhost;port=3306;mysql_read_default_file=~/.my.cnf;mysql_read_default_group=client";
my $dbc = DBI->connect("$dsn", "heinemann", "MariaDB2022Access",{RaiseError => 1, PrintError => 1, mysql_enable_utf8 => 1});

=begin $PGCMD
  PostgreSQL CMD for Import-Helper command!
=cut
my $PGCMD = "pg-heinemann";

my $IMAGE_LOCATION = "/home/heinemann/Developement/antiqua/database/tmp/BildDaten";


=begin
  Schreibe die Tabelle in Datei für Script psql-migration.sh
=cut
sub init_image_file {
  open(FH, '>:encoding(UTF-8)', "./image-migration.sql") or die $!;
    print FH "TRUNCATE inventory_images;\n";
  close(FH);
};

sub add_image_file {
  my $_t = shift;
  if(length($_t)>4) {
    open(FH, '>>:encoding(UTF-8)', "./image-migration.sql") or die $!;
    print FH $_t ."\n";
    close(FH);
  }
};

sub scale_image {
  my $_f = shift;
  system("rm -f /tmp/out.jpg");
  copy($_f,"/tmp/import.jpg");
  my $exifTool = new Image::ExifTool;
  my $info = $exifTool->ImageInfo("/tmp/import.jpg");
  my $rotate = $exifTool->GetValue('Orientation');
  $rotate =~ s/\D+//ig;
  $exifTool->ClearOptions();
  $rotate = (($rotate) && ($rotate >= 90)) ? $rotate : 0;
  ## Convert
  my @args = ("convert","-resize","320x","-rotate",$rotate,"-quality",80,"/tmp/import.jpg","/tmp/out.jpg");
  system(@args) == 0
  or system("rm -f /tmp/out.jpg");

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
    if ($_id =~ qr/^\d{1,6}\.jpg$/i) {
      print "-- $_id\n";
      my $_img = scale_image($file);
      if($_img) {
        my $sql = "INSERT INTO inventory_images (im_id,im_imgdata,im_changed) VALUES ";
        $_id =~ s/\D+//ig;
        $_img =~ s/[\n\r]+//ig;
        $sql .= "($_id,'$_img',CURRENT_TIMESTAMP);\n";
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

=begin disconnect
  Aufräumen und SQL Verbindungen schliessen.
=cut
$dbc->disconnect();

# print "-- script finished\n";
##EOF
