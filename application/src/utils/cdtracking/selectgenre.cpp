// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectgenre.h"

#include <QDebug>

SelectGenre::SelectGenre(QWidget *parent) : QComboBox{parent} {
  setObjectName("id3_tag_selecter");

  m_search = new QLineEdit(this);
  m_search->setPlaceholderText(tr("Without disclosures"));
  setLineEdit(m_search);

  reload();

  connect(m_search, SIGNAL(textChanged(const QString &)),
          SLOT(find(const QString &)));
}

void SelectGenre::find(const QString &s) { qDebug() << Q_FUNC_INFO << s; }

void SelectGenre::reload() {
  clear();
  insertItem(0, QString());
  int c = 1;
  QStringList tags = id3Tags();
  foreach (QString genre, tags) {
    insertItem(c++, genre, genre);
  }
  m_cpl = new QCompleter(tags, m_search);
  m_cpl->setCaseSensitivity(Qt::CaseInsensitive);
  m_cpl->setFilterMode(Qt::MatchContains);
  setCompleter(m_cpl);
}

const QStringList SelectGenre::id3Tags() {
  QStringList list;
  list.append(tr("Abstract"));
  list.append(tr("Acapella"));
  list.append(tr("Acid"));
  list.append(tr("Acid Jazz"));
  list.append(tr("Acid Punk"));
  list.append(tr("Acoustic"));
  list.append(tr("Alternative"));
  list.append(tr("AlternRock"));
  list.append(tr("Ambient"));
  list.append(tr("Anime"));
  list.append(tr("Art Rock"));
  list.append(tr("Audiobook"));
  list.append(tr("Audio Theatre"));
  list.append(tr("Avantgarde"));
  list.append(tr("Ballad"));
  list.append(tr("Baroque"));
  list.append(tr("Bass"));
  list.append(tr("Beat"));
  list.append(tr("Bebob"));
  list.append(tr("Bhangra"));
  list.append(tr("Big Band"));
  list.append(tr("Big Beat"));
  list.append(tr("Black Metal"));
  list.append(tr("Bluegrass"));
  list.append(tr("Blues"));
  list.append(tr("Booty Bass"));
  list.append(tr("Breakbeat"));
  list.append(tr("BritPop"));
  list.append(tr("Cabaret"));
  list.append(tr("Celtic"));
  list.append(tr("Chamber Music"));
  list.append(tr("Chanson"));
  list.append(tr("Chillout"));
  list.append(tr("Chorus"));
  list.append(tr("Christian Gangsta Rap"));
  list.append(tr("Christian Rap"));
  list.append(tr("Christian Rock"));
  list.append(tr("Classical"));
  list.append(tr("Classic Rock"));
  list.append(tr("Club"));
  list.append(tr("Club House"));
  list.append(tr("Comedy"));
  list.append(tr("Contemporary Christian"));
  list.append(tr("Country"));
  list.append(tr("Crossover"));
  list.append(tr("Cult"));
  list.append(tr("Dance"));
  list.append(tr("Dance Hall"));
  list.append(tr("Darkwave"));
  list.append(tr("Death Metal"));
  list.append(tr("Disco"));
  list.append(tr("Downtempo"));
  list.append(tr("Dream"));
  list.append(tr("Drum and Bass"));
  list.append(tr("Drum Solo"));
  list.append(tr("Dub"));
  list.append(tr("Dubstep"));
  list.append(tr("Duet"));
  list.append(tr("Easy Listening"));
  list.append(tr("EBM"));
  list.append(tr("Eclectic"));
  list.append(tr("Electro"));
  list.append(tr("Electroclash"));
  list.append(tr("Electronic"));
  list.append(tr("Emo"));
  list.append(tr("Ethnic"));
  list.append(tr("Eurodance"));
  list.append(tr("Euro house"));
  list.append(tr("Euro Techno"));
  list.append(tr("Experimental"));
  list.append(tr("Fast Fusion"));
  list.append(tr("Folk"));
  list.append(tr("Folklore"));
  list.append(tr("Folk Rock"));
  list.append(tr("Freestyle"));
  list.append(tr("Funk"));
  list.append(tr("Fusion"));
  list.append(tr("Game"));
  list.append(tr("Gangsta"));
  list.append(tr("Garage"));
  list.append(tr("Garage Rock"));
  list.append(tr("G Funk"));
  list.append(tr("Global"));
  list.append(tr("Goa"));
  list.append(tr("Gospel"));
  list.append(tr("Gothic"));
  list.append(tr("Gothic Rock"));
  list.append(tr("Grunge"));
  list.append(tr("Hardcore"));
  list.append(tr("Hard Rock"));
  list.append(tr("Heavy Metal"));
  list.append(tr("Hip Hop"));
  list.append(tr("House"));
  list.append(tr("Humour"));
  list.append(tr("IDM"));
  list.append(tr("Illbient"));
  list.append(tr("Indie"));
  list.append(tr("Indie Rock"));
  list.append(tr("Industrial"));
  list.append(tr("Industro Goth"));
  list.append(tr("Instrumental"));
  list.append(tr("Instrumental Pop"));
  list.append(tr("Instrumental Rock"));
  list.append(tr("Jam Band"));
  list.append(tr("Jazz"));
  list.append(tr("Jazz+Funk"));
  list.append(tr("JPop"));
  list.append(tr("Jungle"));
  list.append(tr("Krautrock"));
  list.append(tr("Latin"));
  list.append(tr("Leftfield"));
  list.append(tr("Lo Fi"));
  list.append(tr("Lounge"));
  list.append(tr("Math Rock"));
  list.append(tr("Meditative"));
  list.append(tr("Merengue"));
  list.append(tr("Metal"));
  list.append(tr("Musical"));
  list.append(tr("National Folk"));
  list.append(tr("Native American"));
  list.append(tr("Negerpunk"));
  list.append(tr("Neoclassical"));
  list.append(tr("Neue Deutsche Welle"));
  list.append(tr("New Age"));
  list.append(tr("New Romantic"));
  list.append(tr("New Wave"));
  list.append(tr("Noise"));
  list.append(tr("Nu Breakz"));
  list.append(tr("Oldies"));
  list.append(tr("Opera"));
  list.append(tr("Other"));
  list.append(tr("Podcast"));
  list.append(tr("Polka"));
  list.append(tr("Polsk Punk"));
  list.append(tr("Pop"));
  list.append(tr("Pop Folk"));
  list.append(tr("Pop Funk"));
  list.append(tr("Porn Groove"));
  list.append(tr("Post Punk"));
  list.append(tr("Post Rock"));
  list.append(tr("Power Ballad"));
  list.append(tr("Pranks"));
  list.append(tr("Primus"));
  list.append(tr("Progressive Rock"));
  list.append(tr("Psybient"));
  list.append(tr("Psychedelic"));
  list.append(tr("Psychedelic Rock"));
  list.append(tr("Psytrance"));
  list.append(tr("Punk"));
  list.append(tr("Punk Rock"));
  list.append(tr("Rap"));
  list.append(tr("Rave"));
  list.append(tr("Rhythm and Blues"));
  list.append(tr("Reggae"));
  list.append(tr("Reggaestep"));
  list.append(tr("Retro"));
  list.append(tr("Revival"));
  list.append(tr("Rhythmic Soul"));
  list.append(tr("Rock"));
  list.append(tr("Rock and Roll"));
  list.append(tr("Salsa"));
  list.append(tr("Samba"));
  list.append(tr("Satire"));
  list.append(tr("Shoegaze"));
  list.append(tr("Showtunes"));
  list.append(tr("Ska"));
  list.append(tr("Slow Jam"));
  list.append(tr("Slow Rock"));
  list.append(tr("Sonata"));
  list.append(tr("Soul"));
  list.append(tr("Sound Clip"));
  list.append(tr("Soundtrack"));
  list.append(tr("Southern Rock"));
  list.append(tr("Space"));
  list.append(tr("Space Rock"));
  list.append(tr("Speech"));
  list.append(tr("Swing"));
  list.append(tr("Symphonic Rock"));
  list.append(tr("Symphony"));
  list.append(tr("SynthPop"));
  list.append(tr("Tango"));
  list.append(tr("Techno"));
  list.append(tr("Techno Industrial"));
  list.append(tr("Terror"));
  list.append(tr("Thrash Metal"));
  list.append(tr("Top 40"));
  list.append(tr("Trailer"));
  list.append(tr("Trance"));
  list.append(tr("Tribal"));
  list.append(tr("Trip Hop"));
  list.append(tr("Trop Rock"));
  list.append(tr("Vocal"));
  list.append(tr("World Music"));
  return list;
}

const QString SelectGenre::genres() {
  int index = currentIndex();
  if (index < 1)
    return QString();

  QString buffer = itemData(index, Qt::UserRole).toString();
  buffer.replace(" ", ",");
  return buffer.trimmed();
}
