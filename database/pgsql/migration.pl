#!/usr/bin/env perl

=head1 NAME

  migrate.pl - Perl Script to Migrate the HAI Database System

=cut

use strict;
use warnings;
use POSIX qw(strftime);
use DBI;
use Text::Roman qw(:all);
use Switch;

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

=begin trim
  Einfache trim() Funktion im Posix Style
=cut
sub trim {
  my $_s = shift;
  $_s =~ s/^\s+|\s+$//g;
  $_s =~ s/(\t|\s)+/ /ig;
  return $_s;
};

=begin
  Schreibe die Tabelle in Datei für Script psql-migration.sh
=cut
sub clear_migration_order {
  open(FH, '>:encoding(UTF-8)', "./psql-migration-order.txt") or die $!;
  close(FH);
};

sub add_migration_order {
  my $_t = shift;
  if(length($_t)>4) {
    open(FH, '>>:encoding(UTF-8)', "./psql-migration-order.txt") or die $!;
    print FH trim($_t)."\n";
    close(FH);
  }
};

=begin perpare_punctuation
  Verschieden Satzezeichen an die richtige Position schieben.
=cut
sub perpare_punctuation {
  my $_s = shift;
  ## Satzzeichen heranschieben.
  $_s =~ s/(\(\s)/(/g;
  $_s =~ s/(\s\))/)/g;
  $_s =~ s/(\s\/\s)/\/ /g;
  $_s =~ s/(\s\?)/?/g;
  $_s =~ s/(\s\,)/,/g;
  $_s =~ s/(\s\!)/!/g;
  return $_s;
};

=begin remove_invalid
  1) Tabulaturen durch Leerzeichen ersetzen.
  2) Zeilenumbrüche durch Leerzeichen ersetzen.
  3) Route (#) entfernen
=cut
sub remove_invalid {
  my $_s = shift;
  ## Tabulaturen und Zeilenumbrüche entfernen
  $_s =~ s/(\t|\r\n|\r|\n|#)+/ /mg;
  $_s =~ s/(\'|\"|\>|\<|#)+//mg;
  ## Einträge wie o.A. oder o.J. weg!
  $_s =~ s/([a-z]{1}(\.|\,)[a-z]{1}(\.|\,){0,1})//ig;
  return $_s;
};

=begin prepare_text
  1) Tabulaturen durch Leerzeichen ersetzen.
  2) Zeilenumbrüche durch Leerzeichen ersetzen.
  3) Mehrfache Leerzeichen vereinfachen.
=cut
sub prepare_text {
  my $_s = shift;
  $_s = remove_invalid($_s);
  $_s = perpare_punctuation($_s);
  $_s =~ s/(\s)+/ /mg;
  return ucfirst $_s;
};

=begin prepare_condition
  Zustandsbeschreibung
=cut
sub prepare_condition {
  my $_s = shift;
  my $_o;
  $_s = trim($_s);
  $_s = remove_invalid($_s);
  $_s =~ s/\s\/\s/,/mg;
  $_s =~ s/\//, /mg;
  $_s =~ s/\s+/ /mg;
  $_s =~ s/katoniert/kartoniert/ig;
  return ucfirst $_s;
};

=begin prepare_pagecount
  Falscher Datentyp bei Seitenanzahl im HAI-System.
  Die Funktion entfernt unerlaubte Zeichensätze und Multipiliziert
  bei einer Mehrfachen Seitenzahl zu einer Gesamtsumme.
  Der Datentyp ist "integer" und gibt die gesamte Zeitenzahl aller Werke an!
=cut
sub prepare_pagecount {
  my $_str = shift;
  ## https://perldoc.perl.org/perlre
  $_str =~ s/#/ /g;
  ## 1. Alle NICHT Nummern entfernen
  $_str =~ s/\D+/ /g;
  ## 2. trim
  $_str = trim($_str);
  ## 3. Leerzeichen durch Komma ersetzen!
  $_str =~ s/\s/,/g;
  ## Nehme die Summe wenn mehrfach Vorhanden.
  if(index($_str,',') != -1)
  {
    my $c = 0;
    foreach (split(',',$_str))
    {
      $c += $_;
    }
    return sprintf("%d",$c);
  }
  ## Ausgeben
  return (length($_str) gt 0) ? sprintf("%d",$_str) : 0;
};

=begin prepare_title_extended, prepare_book_title
  Der Buchtitel ist bei allen Online Portalen auf 80 Zeichen begrenzt.
  Weil die alten Einträge fehlerhaft und mit einem falschen Datetyp
  versehen sind. Wird hier ein Workaround angewendet der möglichst
  sanft das ganze auf die 80 Zeichen herunter schneidet!
  - Es werden doppelte Leerzeichen, Tabulaturen und Zeilenumbrüche entfernt.
  - Anhand eines Bindestriches wird nach Satzbildungen gesucht.
  - Sollte der Text immer noch zu lang sein, widerholt sich der Vorgang.
=cut
sub prepare_title_extended {
  my $_str = shift;
  $_str = remove_invalid($_str);
  ## Unzulässige Ausrufezeichen!
  $_str =~ s/\"/ /g;
  $_str =~ s/\'/"/g;
  ## Leerzeichen entfernen
  $_str = trim($_str);
  $_str = perpare_punctuation($_str);
  ## Klammern zusammen rücken
  $_str =~ s/\(\s+/(/g;
  $_str =~ s/\s+\)/)/g;
  ## Mehrfache Leerzeichen ersetzen
  $_str =~ s/\s+/ /g;
  ## Datumsangaben Korrigieren (Jahr-Jahr)
  $_str =~ s/\b\-\s(?=\d)/-/g;
  ## Alles hinter einem Bindestrich muss weg!
  if(index($_str,'-',70)>0) {
    $_str =~ s/\s\-\s.+$//g;
  }
  ## Bindestrich an Wortgrenze verschieben
  $_str =~ s/\-(?=[a-z])/- /ig;
  ## Ein Buchstabe am Ende entfernen.
  ## BUG Erst ab 80 Zeichen verwenden!
  if(length($_str)>79) {
    $_str =~ s/\s+(\({0,1}\w{1,2})$//g;
  }
  ## Leerzeichen entfernen
  $_str = trim($_str);
  ## Abschneiden wenn zu lang
  if(length($_str)>148)
  {
    my @words = split / /, $_str;
    pop @words;
    $_str = join(" ", @words);
    $_str = prepare_title_extended($_str);
  }
  return $_str;
};

sub prepare_book_title {
  my $_str = shift;
  $_str = prepare_title_extended($_str);
  ## Abschneiden wenn zu lang
  if(length($_str)>80)
  {
    my @words = split / /, $_str;
    pop @words;
    $_str = join(" ", @words);
    $_str = prepare_book_title($_str);
  }
  return $_str;
};

=begin prepare_author
  Wenn keine Daten vorhanden dann wird "NOT_SET" verwendet weil der Datentyp
  in der PostgreSQL Datenbank auf "NOT NULL" steht!
  NOTE - Das muss später im UI Abgefangen werden!
  - Es werden doppelte Leerzeichen, Tabulaturen und Zeilenumbrüche entfernt.
  - Anhand eines Bindestriches wird nach Satzbildungen gesucht.
=cut
sub prepare_author {
  my $_str = shift;
  $_str = remove_invalid($_str);
  ## Mehrfache Leerzeichen ersetzen
  $_str =~ s/\s+/ /g;
  ## o.A. entfernen
  $_str =~ s/o\.a\.{0,1}//ig;
  $_str =~ s/(\'|\"|\>|\<|#)+//mg;
  ## Leerzeichen entfernen
  $_str = trim($_str);
  if(length($_str)<1)
  {
    return "NOT_SET"
  }
  return $_str;
};

=begin prepare_year
  Falsche Eingabe oder Datentyp bei Jahr.
  Die PostgreSQL Datenbank verwendet NUMERIC(4,0)
  Das Jahr besteht aus 4 Zahlen und darf nicht abweichen.

  SELECT input AS year FROM generate_series(1900,9999,1000) AS input;

=cut
sub prepare_year {
  my $_str = shift;
  $_str =~ s/(\D)+//g;
  $_str = substr($_str,0,4);
  if (length($_str)>4)
  {
    $_str =~ s/\D+/#/ig;
    $_str =~ s/#.+$//ig;
  }
  return (length($_str) == 4) ? sprintf("%d",$_str) : 0;
};

=begin prepare_check_isbn
  Prüfe auf ISBN/EAN nach ISO 2108

  SELECT REPLACE(isbn,'-','') AS i_isbn FROM xgr
    WHERE length(isbn) > 9 AND isbn NOT REGEXP '[a-zA-Z]+' ORDER BY artnr;

  Entferne Leerzeichen und alles was nicht eine Zahl ist.
  Prüfe dann auf die erforderliche Zahlenlänge (10 oder 13).
  - Wenn diese nicht stimmen, sofort verwerfen.

  Ist der 1. Teil bei einer Länge von 13 das Präfix 978 oder 979?
  - Wenn nicht, sofort verwerfen.

  Im UI eine Erweiterung schreiben mit der ISBN Nummern Online abgerufen werden können.

  Siehe OpenLibrary https://openlibrary.org/dev/docs/api/books

  print "curl 'http://openlibrary.org/api/books?bibkeys=ISBN:${_isbn}&jscmd=data&format=json'\n";
  print "https://openlibrary.org/isbn/${_isbn}.json\n"

  Alternativen isbnsearch.org oder die Google Books API
  Beispielseite für die Google Books API Nutzung:
    https://www.labnol.org/code/20020-query-book-by-isbn

  Kostenpflichtig:
    https://isbndb.com/apidocs/v2

=cut
sub prepare_check_isbn {
  my $_isbn = shift;
  ## Verschiedene Tests durchlaufen.
  $_isbn = trim($_isbn);
  ## Alles was keine Zahl ist entfernen!
  $_isbn =~ s/\D+//g;

  ## Keine ISBN10 oder 13
  if (length($_isbn) < 10) {
    return 0;
  }

  if (length($_isbn) > 13) {
    return 0;
  }

  ## ISBN13 Check
  my $prefix = substr($_isbn,0,3);
  if (($prefix != 978) or !($prefix != 979))
  {
    ## ISBN10 Check
    if(substr($_isbn,0,1) != 3)
    {
      return 0;
    }
  }
  return sprintf("%d",$_isbn);
};

=begin prepare_format
  Format Ausgabe Normalisieren
  - Leerzeichen weg
  - Zentimeter Angaben weg
=cut
sub prepare_format {
  my $_f = shift;
  $_f =~ s/[a-w]+//ig;
  $_f =~ s/\s+//ig;
  $_f =~ s/X/x/g;
  $_f = trim($_f);
  return $_f;
};

=begin show_table_fields
  Einfache Tabellenfelder auflistung!
=cut
sub show_table_fields {
  my $table = shift;
  my $query = $dbc->prepare("SHOW FIELDS FROM $table;");
  if($query->execute())
  {
    while (my $ref = $query->fetchrow_hashref())
    {
      print "$ref->{'Field'} | $ref->{'Type'}\n";
    }
    $query->finish;
  }
};

=begin create_article_inserts
  Importiert die komplette Liste aller existierenden Artikel ID's
=cut
sub create_article_inserts {
  my $table = "public.inventory";
  my $query = $dbc->prepare("SELECT artnr,zeitstempel FROM xgr WHERE artnr>0 ORDER BY artnr ASC;");
  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${table}.INSERT");
    print "-- Generate: ${table}.INSERT\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.INSERT.sql") or die $!;
    print FH "--\n-- PostgreSQL INSERT INTO $table --\n--\n\n";
    print FH "TRUNCATE $table RESTART IDENTITY CASCADE;\n\n";
    my $inserts = 0;
    while (my $ref = $query->fetchrow_hashref())
    {
      print FH "INSERT INTO $table (i_id,i_since) OVERRIDING SYSTEM VALUE VALUES ($ref->{'artnr'},'$ref->{'zeitstempel'}');\n";
      $inserts++;
    }
    $query->finish;
    print "-- DONE: From $rows rows, created $inserts Storage inserts.\n";
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
  }
};

=begin create_storage_locations
  Antiquariat Lagerpositionen importieren.
  Änderungen zur Vorversion:
  - Ab sofort wird mit eine Zahl indiziert.
  - Den Wert 0 für "Kein Zuordnung" hinzu gefügt!
  Warum jetzt ID's - Zahlen?
  - Nicht jedes Antiquariat besitzt das gleiche Lagersystem!
  - Hiermit wird eine höhere Flexibilität erreicht!
  - So lange keine ID's gelöscht werden kann das Lagersystem jederzeit geändert werden.
=cut
sub create_storage_locations {
  my $table = "public.ref_storage_location";
  my $query = $dbc->prepare("SELECT lagerort AS sl_storage,bezeichnung AS sl_identifier FROM hai.xgrlagerort ORDER BY lagerort ASC;");
  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${table}.INSERT");
    print "-- Generate: ${table}.INSERT\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.INSERT.sql") or die $!;
    print FH "--\n-- PostgreSQL INSERT INTO $table --\n--\n\n";
    print FH "TRUNCATE $table RESTART IDENTITY CASCADE;\n\n";
    print FH "INSERT INTO $table (sl_id,sl_storage,sl_identifier) OVERRIDING SYSTEM VALUE VALUES (0,'OPEN','Keine Zuordnung');\n";

    my $inserts = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'sl_storage'} && $r->{'sl_identifier'})
      {
        my $location = ucfirst trim($r->{'sl_storage'});
        my $identifier = ucfirst trim($r->{'sl_identifier'});
        print FH "INSERT INTO $table (sl_storage,sl_identifier,sl_location) VALUES ('$location','$identifier','Marktstr. 14');\n";
        $inserts++;
      }
    }
    $query->finish;
    print "-- DONE: From $rows rows, created $inserts Storage inserts.\n";
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
  }
};

=begin create_inventory_books
  Buchdaten aufbereiten.
=cut
sub create_inventory_books {
  my $table = "public.inventory_books";

  my $query = $dbc->prepare("SELECT
  xgr.artnr AS ib_id,
  IFNULL(xgr.ort,'NOT_SET') AS ib_author,
  LEFT(xgr.titel,80) AS ib_title,
  LEFT(xgr.titel,148) AS ib_title_extended,
  xgr.jahr AS ib_year,
  LEFT(xgr.erhalt,128) AS ib_condition,
  xgr.herausgeber AS ib_description,
  REPLACE(xgr.isbn,'-','') AS ib_isbn,
  xgr.stueck AS ib_count,
  xgr.format AS ib_pagecount,
  IFNULL(xgr.vk_preis,0.00) AS ib_price,
  IFNULL(xgr.gewicht,0) AS ib_weight,
  xgr.zeitstempel AS ib_changed
FROM xgr
WHERE xgr.art LIKE 'bu' AND xgr.artnr>0 AND length(xgr.titel)>1
ORDER BY xgr.artnr ASC;
");

  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${table}.INSERT");
    print "-- Generate: ${table}.INSERT\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.INSERT.sql") or die $!;
    print FH "--\n-- PostgreSQL INSERT INTO $table --\n--\n\n";
    print FH "TRUNCATE $table CASCADE;\n\n";

    my $created = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if(!$r->{'ib_id'}) { continue; }
      my $insert = "ib_id,";
      my $values = "$r->{'ib_id'},";
      ## NOTE Geht hier nicht anders weil auf SET NOT NULL!
      my $author = ($r->{'ib_author'}) ? prepare_author($r->{'ib_author'}) : "NOT_SET";
      if (length($author) > 127) {
        die "Zeilengrenze überschritten (".length($author).")!\n SELECT titel,ort FROM xgr WHERE artnr=$r->{'ib_id'};";
      }
      $insert .= "ib_author,";
      $values .= "'$author',";
      ## Buchtitel Max 80-148 Zeichen
      my $title = "";
      if($r->{'ib_title'})
      {
        $title = prepare_book_title($r->{'ib_title'});
        $insert .= "ib_title,";
        $values .= "'$title',";
        ## Titel erweitert mit Max 148 Zeichen
        $title = ($r->{'ib_title_extended'}) ? prepare_title_extended($r->{'ib_title_extended'}) : "";
        if(length($title) > 80) {
          $insert .= "ib_title_extended,";
          $values .= "'$title',";
        }
      } else {
        die "-- FATAL NO Title from artnr=$r->{'ib_id'} AND titel='$r->{'ib_title'}' | title='$title' --\n";
        continue;
      }
      ## Jahr numeric(4)
      my $year =  ($r->{'ib_year'}) ? prepare_year($r->{'ib_year'}) : 0;
      if($year > 0) {
        $insert .= "ib_year,";
        $values .= "$year,";
      }
      ## Zustands Beschreibung
      my $condition = ($r->{'ib_condition'}) ? prepare_condition($r->{'ib_condition'}) : "";
      if(length($condition) gt 3) {
        $insert .= "ib_condition,";
        $values .= "'$condition',";
      }
      ## Buch Beschreibung
      my $description = ($r->{'ib_description'}) ? prepare_text($r->{'ib_description'}) : "";
      if(length($description) gt 3) {
        $insert .= "ib_description,";
        $values .= "'$description',";
      }
      ## ISBN
      ## NOTE - Ist jetzt ein bigint Wert
      my $isbn = ($r->{'ib_isbn'}) ? prepare_check_isbn($r->{'ib_isbn'}) : 0;
      if($isbn != 0) {
        $insert .= "ib_isbn,";
        $values .= "$isbn,";
      }
      ## Anzahl im Bestand
      ## WARNING Keine Negativen Werte zulassen
      my $count = ($r->{'ib_count'}) ? $r->{'ib_count'} : 0;
      if($count gt 0) {
        $insert .= "ib_count,";
        $values .= "$count,";
      }
      ## Lagerpositionen
      ## @see update_book_storage_location
      $insert .= "ib_storage,";
      $values .= "0,";
      ## Seitenanzahl
      my $pages = ($r->{'ib_pagecount'}) ? prepare_pagecount($r->{'ib_pagecount'}) : 0;
      if($pages > 0) {
        $insert .= "ib_pagecount,";
        $values .= "$pages,";
      }
      ## Buchpreis
      my $price = sprintf("%.02f",$r->{'ib_price'});
      $insert .= "ib_price,";
      $values .= "$price,";
      ## Gewicht für Versand
      my $weight = sprintf("%d",$r->{'ib_weight'});
      $insert .= "ib_weight,";
      $values .= "$weight,";
      ## Datum
      if($r->{'ib_changed'}) {
        my $timestamp = $r->{'ib_changed'};
        $insert .= "ib_changed";
        $values .= "'$timestamp'";
      } else {
        $insert .= "ib_changed";
        $values .= "now()";
      }
      ## Schreiben
      print FH "INSERT INTO $table ($insert) VALUES ($values);\n";
      $created++;
    }
    $query->finish;
    print "-- DONE: From $rows rows, created $created Book inserts.\n";
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
  }
};

=begin create_update_book_isbn_entries
  Im Feld info sind ISBN Nummern abgelegt!
  - Diese Extrahieren!
=cut
sub create_update_book_isbn_entries {
  my $booktb = "public.inventory_books";
  my $query = $dbc->prepare("SELECT artnr AS ib_id,
  REPLACE(REPLACE(REPLACE(info,'ISBN',''),'-',''),' ','') AS ib_isbn
FROM xgr WHERE artnr>0 AND info LIKE 'ISBN%' ORDER BY artnr;"
);

  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${booktb}.UPDATE.ib_isbn");
    print "-- Generate: ${booktb}.UPDATE.ib_isbn\n";
    open(FH, '>:encoding(UTF-8)', "./import/${booktb}.UPDATE.ib_isbn.sql") or die $!;
    print FH "--\n-- PostgreSQL UPDATE SET $booktb --\n--\n";
    my $updates = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'ib_id'})
      {
        my $isbn = ($r->{'ib_isbn'}) ? prepare_check_isbn($r->{'ib_isbn'}) : 0;
        if($isbn > 0)
        {
          print FH "UPDATE $booktb SET ib_isbn=$isbn WHERE ib_isbn IS NULL AND ib_id=$r->{'ib_id'};\n";
          $updates++;
        }
      }
    }
    $query->finish;
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
    print "-- DONE: Query $rows rows and UPDATE $updates Book ISBN entries.\n";
  }
};

=begin create_update_book_storagelocation
  Suche alte Lagerorte und korrigiere die Beziehungen zum neuen Lagersystem.
  Ab sofort sind die Lagerangaben ein INT Zeiger auf die Tabelle "storage_location".
=cut
sub create_update_book_storagelocation {
  my $booktb = "public.inventory_books";
  my $stortb = "public.ref_storage_location";
  my $query = $dbc->prepare("SELECT artnr AS id,lagerort AS la,titel FROM xgr WHERE art LIKE 'bu' ORDER BY artnr;");
  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${booktb}.UPDATE.ib_storage");
    print "-- Generate: ${booktb}.UPDATE.ib_storage\n";
    open(FH, '>:encoding(UTF-8)', "./import/${booktb}.UPDATE.ib_storage.sql") or die $!;
    print FH "--\n-- PostgreSQL UPDATE SET $booktb --\n--\n";
    my $updates = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'la'} && ($r->{'id'} > 0))
      {
        print FH "UPDATE $booktb SET ib_storage=(SELECT sl_id FROM $stortb WHERE sl_storage='$r->{'la'}') WHERE ib_id=$r->{'id'};\n";
        $updates++;
      }
    }
    $query->finish;
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
    print "-- DONE: Query $rows rows and UPDATE $updates Book entries.\n";
  }
};

=begin create_update_book_volumes
  Weil sich die Buchband angeben zum Teil im Feld technik befinden.
  Hier ein Komplettes Polishing auf die Datensätze.
=cut
sub create_update_book_volumes {
  my $table = "public.inventory_books";
  my $query = $dbc->prepare("SELECT artnr AS id, technik
FROM xgr WHERE art LIKE 'bu' AND technik LIKE '%Bd.%' OR technik LIKE '%Auflage' ORDER BY artnr;");
  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${table}.UPDATE.ib_volume");
    print "-- Generate: ${table}.UPDATE.ib_volume\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.UPDATE.ib_volume.sql") or die $!;
    print FH "--\n-- PostgreSQL UPDATE SET $table --\n--\n";
    my $updates = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'technik'})
      {
        my $vol = "$r->{'technik'}";
        $vol =~ s/auflage//ig;
        $vol =~ s/Bd//ig;
        $vol =~ s/\.+//g;
        $vol =~ s/\s+//g;
        if ($vol =~ m/[0-9]+/) {
          $vol = sprintf("%d",$vol);
          print FH "UPDATE $table SET ib_volume=${vol} WHERE ib_id=$r->{'id'};\n";
          $updates++;
        } elsif (isroman($vol)) {
          my $vol = roman2int($vol);
          print FH "UPDATE $table SET ib_volume=${vol} WHERE ib_id=$r->{'id'};\n";
          $updates++;
        }
      }
    }
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
    print "-- DONE: Query $rows rows and UPDATE $updates Book ib_volumes.\n";
    $query->finish;
  }
};

=begin create_inventory_prints
  Drucke, Bilder, Stiche und Fotos sind jetzt
  in Tabelle public.inventory_prints enthalten.
  - Suche mit art=
=cut
sub create_inventory_prints {
  my $table = "public.inventory_prints";
  my $query = $dbc->prepare("SELECT
  artnr,
  art,
  IFNULL(stueck,0) AS co,
  titel,
  IFNULL(ort,'') AS ort,
  von,
  kolorit,
  technik,
  jahr,
  format,
  querhoch,
  erhalt,
  zusatz, 
  vk_preis,
  FORMAT(vk_preis,2,'POSIX') AS price,
  info
FROM hai.xgr
WHERE xgr.art LIKE 'po' OR xgr.art LIKE 'an' OR xgr.art LIKE 'ka' AND artnr>0
ORDER BY artnr ASC;");
  my $rows = $query->execute();
  if($rows)
  {
    my $insert = "";
    my $values = "";
    my $inserts = 0;
    add_migration_order("${table}.INSERT");
    print "-- Generate: ${table}.INSERT\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.INSERT.sql") or die $!;
    print FH "--\n-- PostgreSQL UPDATE SET $table --\n--\n";
    print FH "TRUNCATE $table CASCADE;\n\n";

    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'artnr'})
      {
        ## Referenz ID integer
        $insert = "ip_id";
        $values = sprintf("%d",$r->{'artnr'});
        ## Anzahl smallint
        $insert .= ",ip_count";
        $values .= "," . sprintf("%d",$r->{'co'}) . "";
        ## ip_title character varying(80)
        if($r->{'titel'})
        {
          $insert .= ",ip_title";
          $values .= ",'". prepare_book_title($r->{'titel'}) ."'";
          ## ip_title_extended character varying(148)
          $insert .= ",ip_title_extended";
          $values .= ",'". prepare_title_extended($r->{'titel'}) ."'";
        }
        ## ip_author character varying(128)
        if($r->{'von'})
        {
          my $author = trim($r->{'von'});
          $author = prepare_author($author);
          $insert .= ",ip_author";
          $values .= ",'$author'";
        }
        ## ip_views boolean
        if($r->{'art'})
        {
          my $view = trim($r->{'art'});
          if($view =~ m/an/i)
          {
            $insert .= ",ip_views";
            $values .= ",true";
          }
        }
        ## ip_kolorit boolean
        if($r->{'kolorit'})
        {
          $insert .= ",ip_kolorit";
          $values .= ",true";
        }
        ## ip_format character varying(50)
        if($r->{'format'})
        {
          my $format = prepare_format($r->{'format'});
          $insert .= ",ip_format";
          $values .= ",'$format'";
        }
        ## ip_landscape boolean
        if($r->{'querhoch'})
        {
          $insert .= ",ip_landscape";
          if ($r->{'querhoch'} =~ qr/quer/i) {
            $values .= ",true";
          } else {
            $values .= ",false";
          }
        }
        ## ip_year
        if($r->{'jahr'}) {
          $insert .= ",ip_year";
          $values .= "," . prepare_year($r->{'jahr'});
        }
        ## ip_condition character varying(128)
        if($r->{'erhalt'}) {
          $insert .= ",ip_condition";
          $values .= ",'" . prepare_condition($r->{'erhalt'}) . "'";
        }
        ## ip_price numeric(6,2)
        ## $r->{'vk_preis'}#
        if($r->{'price'}) {
          $insert .= ",ip_price";
          $values .= "," . $r->{'price'};
        }
        if($r->{'zusatz'})
        {
          my $info = prepare_text($r->{'zusatz'});
          if(length($info)>127)
          {
            ## ip_description
            $insert .= ",ip_description";
            $values .= ",'$info'";
          } else {
            ## ip_designation character varying(128)
            $insert .= ",ip_designation";
            $values .= ",'$info'";
          }
        } elsif($r->{'info'}) {
          $insert .= ",ip_description";
          $values .= ",'".prepare_text($r->{'info'})."'";
        }

        print FH "INSERT INTO $table ($insert) VALUES ($values);\n";
        $inserts++;
      }
    }
    $query->finish; 
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
    print "-- DONE: Query $rows rows and INSERT $inserts Prints.\n";
  }
};

=begin update_prints_technique
  Weil sich der Datentyp für die Technik von Druck, Stich, Bildern und Fotos geändert hat.
  Update auf die Spalte "ip_technique" mit der Referenz auf Tabelle "ref_print_technique".
  \dS ref_print_technique \dS inventory_prints
  SELECT rpt_id,rpt_type FROM ref_print_technique ORDER BY rpt_id;
  UPDATE inventory_prints SET ip_technique='0';
  ALTER TABLE inventory_prints ALTER COLUMN ip_technique TYPE smallint USING ip_technique::smallint;
  ALTER TABLE inventory_prints ADD FOREIGN KEY (ip_technique) REFERENCES ref_print_technique(rpt_id);
=cut
sub update_prints_technique {
  my $table = "public.inventory_prints";
  my $reftb = "public.ref_print_technique";
  my $query = $dbc->prepare("SELECT artnr AS id, LOWER(technik) AS technik FROM hai.xgr
WHERE art LIKE 'po' OR art LIKE 'an' OR art LIKE 'ka' AND artnr>0
ORDER BY artnr ASC;");
  my $rows = $query->execute();
  if($rows)
  {
    add_migration_order("${table}.UPDATE.ip_technique");
    print "-- Generate: ${table}.UPDATE.ip_technique\n";
    open(FH, '>:encoding(UTF-8)', "./import/${table}.UPDATE.ip_technique.sql") or die $!;
    print FH "--\n-- PostgreSQL UPDATE SET $table --\n--\n";

    my $set = "";
    my $updates = 0;
    while (my $r = $query->fetchrow_hashref())
    {
      if($r->{'id'})
      {
        if($r->{'technik'})
        {
          switch ($r->{'technik'})
          {
            case qr/sta[h]{0,1}lstich/i {
              if($r->{'technik'} =~ qr/[kc]oloriert/i) {
                $set = "stahlstich%koloriert";
              } else {
                $set = "stahlstich";
              }
            } case qr/holzstich/i {
              if($r->{'technik'} =~ qr/[kc]oloriert/i) {
                $set = "holzstich%koloriert";
              } else {
                $set = "holzstich";
              }
            } case qr/kupferstich/i {
              if($r->{'technik'} =~ qr/[kc]oloriert/i) {
                $set = "kupferstich%koloriert";
              } else {
                $set = "kupferstich";
              }
            } case qr/holzschnitt/i {
              if($r->{'technik'} =~ qr/[kc]oloriert/i) {
                $set = "holzschnitt%koloriert";
              } else {
                $set = "holzschnitt";
              }
            } case qr/radierung/i {
              $set = 'radierung';
            } case qr/lithographie/i {
              if($r->{'technik'} =~ qr/([kc]oloriert|farblithographie)/i) {
                $set = "lithographie%koloriert";
              } else {
                $set = "lithographie";
              }
            } else {
              die "Unknown Techniques $r->{'technik'}\n";
            }
          };
          print FH "UPDATE $table SET ip_technique=(SELECT rpt_id FROM $reftb WHERE rpt_type ILIKE '$set') WHERE ip_id=$r->{'id'};\n";
          $updates++;
        }
      }
    }
    $query->finish; 
    print FH "--\n-- Query-Result: $rows\n--\n";
    close(FH);
    print "-- DONE: Query $rows rows and Updates $updates created.\n";
  }
};

=begin test_query
  Test Funktion für Verschiedene abfragen.
=cut
sub test_query {
  my $field = "technik";
  my $query = $dbc->prepare("SELECT DISTINCT LOWER($field) AS $field FROM hai.xgr
WHERE art LIKE 'po' OR art LIKE 'an' OR art LIKE 'ka' AND artnr>0
ORDER BY artnr ASC;"
);
  if($query->execute())
  {
    while (my $ref = $query->fetchrow_hashref())
    {
      if($ref->{$field}) {
        print $ref->{$field} . "\n";
      }
    }
    $query->finish;
  }
};

=begin MAIN
  Beginne mit der Script verarbeitung.
=cut

## NOTE Erst Logfile leeren!
clear_migration_order();

## Alle Artikel in public.inventory schreiben!
create_article_inserts();

## Alle Lagerangaben einfügen und ergänzen.
create_storage_locations();

## Jetzt alle Bucheinträge suchen und Konvertieren.
create_inventory_books();

## Weil sich die Lagerort Definition geändert hat jetzt Update durchlaufen.
create_update_book_storagelocation();

## Im Feld info befinden sich ISBN angaben. Diese bei nach nicht vorhanden einfügen.
create_update_book_isbn_entries();

## Die Buchband angaben sind im falschen Feld ...
create_update_book_volumes();

## Starte Drucke, Stiche & Foto import.
create_inventory_prints();

## Nehme Technik ID's aus Tabelle "ref_print_technique"
update_prints_technique();

## Alle Inventar Artikel
# show_table_fields("xgrart");

## Tabellenfelder abfragen
# show_table_fields("xgr");
# show_table_fields("xkd");

## Tests
# test_query();

# print "--\n";
# print "VACUUM VERBOSE ANALYZE public.inventory_books;\n";
# print "ANALYZE VERBOSE public.inventory_prints;\n";

=begin disconnect
  Aufräumen und SQL Verbindungen schliessen.
=cut
$dbc->disconnect();

print "-- script finished\n";
##EOF
