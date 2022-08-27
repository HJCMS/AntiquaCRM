// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebase3.h"
#include "homebasebuilder.h"
#include "homebasereader.h"

#include <QtCore>
#include <QtXml>

// BookListingElement

void HomeBase3::testBuilder() {
  HomeBaseBuilder xmlBuild;
  qDebug() << Q_FUNC_INFO << xmlBuild.isNull();
}

void HomeBase3::testReader(const QString &xmlFile) {
  HomeBaseReader xml;
  if (xml.open(xmlFile)) {
    QFile fp(QDir::homePath() + "/Downloads/abebooks_homebase3.xml");
    if(fp.open(QIODevice::WriteOnly)) {
      QTextStream out(&fp);
      out.setCodec("ISO-8859-1");
      out << xml.toString(1);
      fp.close();
      qDebug() << fp.fileName();
    }
  }
}
