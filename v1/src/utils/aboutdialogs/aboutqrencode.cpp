// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutqrencode.h"

AboutQREncode::AboutQREncode(QWidget *parent) : InfoWidget{parent} {
  setInfoWidget();
}

void AboutQREncode::setInfoWidget() {
  setInfoIcon(QIcon("://icons/filetypes.png"));
  QUrl url;
  url.setScheme("https");
  url.setHost("github.com");
  url.setPath("/fukuchi/libqrencode");
  setHomepage(url);
  setInfoTitle("libqrencode - a fast and compact QR Code encoding library");
  setInfoText("aboutqrencode");
}
