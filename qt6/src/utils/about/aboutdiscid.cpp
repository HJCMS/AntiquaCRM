// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutdiscid.h"

AboutDiscId::AboutDiscId(QWidget *parent) : InfoWidget{parent} {
  setInfoIcon(QIcon("://libdicid.png"));
  QUrl url;
  url.setScheme("https");
  url.setHost("github.com");
  url.setPath("/metabrainz/libdiscid");
  setHomepage(url);
  setInfoTitle("A Library for creating MusicBrainz DiscIDs");
  setInfoText("aboutdiscid");
}
