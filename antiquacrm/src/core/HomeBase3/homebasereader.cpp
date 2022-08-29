// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebasereader.h"

#include <QtCore>
#include <QtXml>

HomeBaseReader::HomeBaseReader() : QDomDocument{} {}

bool HomeBaseReader::open(const QString &xmlFile, bool parse) {
  bool ret = false;
  QFileInfo xml(xmlFile);
  if (xml.isReadable()) {
    QFile fp(xml.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      int errorLine;
      int errorColumn;
      QString errMsg;
      ret = setContent(&fp, true, &errMsg, &errorLine, &errorColumn);
      if (!ret) {
        qDebug() << Q_FUNC_INFO // TODO Logging
                 << errMsg << errorLine << errorColumn;
      }
      fp.close();
    }
  }

  if (ret && documentElement().isNull())
    return false; // document is Empty

  return ret;
}

const QDomNodeList HomeBaseReader::bookListingNodes() {
  if (documentElement().isNull())
    return QDomNodeList();

  return documentElement().childNodes();
}

const BookListing HomeBaseReader::bookListing(const QString &articleId) {
  BookListing element;
  if (articleId.isEmpty())
    return element;

  QDomNodeList nodes = bookListingNodes();
  if (nodes.size() > 0) {
    for (int c = 0; c < nodes.count(); c++) {
      QDomElement e = nodes.at(c).firstChildElement("vendorListingid");
      if (!e.isNull() && e.text().trimmed() == articleId) {
        return BookListing(nodes.at(c).toElement());
      }
    }
  }
  return element;
}
