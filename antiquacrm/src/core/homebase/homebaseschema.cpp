// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "homebaseschema.h"

#include <QDomNodeList>
#include <QtCore>

HomeBaseSchema::HomeBaseSchema() : QDomDocument{} {}

const QString HomeBaseSchema::schemaFile() const {
  QStringList searchDirs;
  searchDirs << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
  searchDirs << QDir::currentPath();

  QFileInfo info;
  foreach (QString p, searchDirs) {
    QString d = QDir::toNativeSeparators(p + "/xml");
    info.setFile(d, HOMEBASE3_BOOKINVENTORY_SCHEMA);
    if (info.isReadable()) {
      return info.filePath();
    }
  }
  return QString();
}

int HomeBaseSchema::getLength(const QDomElement &e, const QString &section) {
  QString s = e.firstChildElement(section).attribute("value", "0");
  bool b;
  int l = s.toInt(&b);
  if (b && l > 0)
    return l;

  return 0;
}

const BookNodeType HomeBaseSchema::fetchType(const QDomElement &element,
                                             const QString &section) {
  QDomElement firstChild = element.firstChildElement(section);
  QDomNodeList nodes = firstChild.elementsByTagName("restriction");
  if (nodes.count() > 0) {
    QDomElement e = nodes.at(0).toElement();
    if (e.attribute("base").contains(":string")) {
      return BookNodeType(section, QMetaType::QString,
                          getLength(e, "minLength"), getLength(e, "maxLength"));
    } else if (e.attribute("base").contains(":nonNegativeInteger")) {
      return BookNodeType(section, QMetaType::Int, getLength(e, "minLength"),
                          getLength(e, "maxLength"));
    } else if (e.attribute("base").contains(":long")) {
      return BookNodeType(section, QMetaType::LongLong,
                          getLength(e, "minLength"), getLength(e, "maxLength"));
    }
  }
  return BookNodeType(section, QMetaType::UnknownType);
}

bool HomeBaseSchema::loadSchema() {
  bool status = false;
  QFile fp(schemaFile());
  if (fp.open(QIODevice::ReadOnly)) {
    int errorLine;
    int errorColumn;
    QString errMsg;
    status = setContent(&fp, true, &errMsg, &errorLine, &errorColumn);
    if (!status) {
      qDebug() << Q_FUNC_INFO // TODO Logging
               << errMsg << errorLine << errorColumn;
    }
    status = (elementsByTagName("schema").count() > 0);
    fp.close();
  }
  return status;
}

const BookNodeType HomeBaseSchema::getPolicy(const QDomElement &base) {
  if (!loadSchema()) {
    qWarning("no schema loaded!");
    return BookNodeType(QString(), QMetaType::UnknownType);
  }

  QDomNodeList nodes = elementsByTagName("element");
  if (nodes.count() > 0) {
    for (int i = 0; i < nodes.count(); i++) {
      QDomElement e = nodes.at(i).toElement();
      if (e.attribute("name") == base.tagName()) {
        p_element = e;
        if (p_element.hasChildNodes()) {
          QDomElement simple = p_element.firstChildElement("simpleType");
          if (!simple.isNull()) {
            BookNodeType t = fetchType(p_element, "simpleType");
            return t;
          }

          QDomElement complex = p_element.firstChildElement("complexType");
          if (!complex.isNull()) {
            BookNodeType t = fetchType(p_element, "complexType");
            return t;
          }
        }
      }
    }
  }
  return BookNodeType(QString(), QMetaType::UnknownType);
}
