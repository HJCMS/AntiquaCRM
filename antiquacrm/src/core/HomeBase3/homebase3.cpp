// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebase3.h"
#include "homebasebuilder.h"
#include "homebasereader.h"

#include <QtCore>
#include <QtXml>

void HomeBase3::testBuilder() {
  HomeBaseBuilder xmlBuilder;
  QString dest(QDir::homePath() + "/Downloads");
  if (xmlBuilder.createDocument())
    xmlBuilder.writeToFile(dest);
}

void HomeBase3::testReader() {
  QString xmlFile(QDir::homePath());
  xmlFile.append("/Developement/antiqua/database/tmp/AbeBooks/abebooks-report.xml");
  HomeBaseReader xml;
  if (xml.open(xmlFile)) {
    QFile fp(QDir::homePath() + "/Downloads/abebooks_homebase3.xml");
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec(HOMEBASE3_DEFAULT_CODEC);
      out << xml.toString(1);
      fp.close();
      qDebug() << "written to:" << fp.fileName();
    }
  }
}
