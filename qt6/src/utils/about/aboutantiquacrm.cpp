// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutantiquacrm.h"

AboutAntiquaCRM::AboutAntiquaCRM(QWidget *parent) : InfoWidget{parent} {
  setInfoIcon(QIcon("://icons/antiquacrm.png"));
  QUrl url;
  url.setScheme("https");
  url.setHost("www.hjcms.de");
  url.setPath("/");
  setHomepage(url);
  setInfoTitle("Antiqua CRM");
  setInfoText("aboutantiquacrm");
}
