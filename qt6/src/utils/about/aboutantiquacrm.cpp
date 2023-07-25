// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aboutantiquacrm.h"
#include "antiquaicon.h"

AboutAntiquaCRM::AboutAntiquaCRM(QWidget *parent) : InfoWidget{parent} {
  setInfoIcon(AntiquaCRM::qrcIcon("antiquacrm"));
  QUrl url;
  url.setScheme("https");
  url.setHost("www.hjcms.de");
  url.setPath("/");
  setHomepage(url);
  setInfoTitle("Antiqua CRM");
  setInfoText("aboutantiquacrm");
}
