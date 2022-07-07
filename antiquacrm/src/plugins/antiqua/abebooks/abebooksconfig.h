// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_CONFIG_H
#define ABEBOOKS_PLUGIN_CONFIG_H

#include <QString>
#include <QByteArray>
#include <QDomDocument>
#include <QTextStream>
#include <QMetaType>
#include <AntiquaInterface>

#ifndef Q_WS_WIN
#ifndef ABEBOOKS_TESTMODE
#define ABEBOOKS_TESTMODE true
#endif

#ifdef ABEBOOKS_TESTMODE
#include <QDomDocument>
#include <QFile>
#include <QDir>
#include <QTextCodec>

static const QDomDocument readSources(const QString &file) {
  QDomDocument out;
  QString xml(QDir::homePath());
  xml.append("/.cache/");
  xml.append(file);
  QFile fp(xml);
  if (fp.open(QIODevice::ReadOnly)) {
    QDomDocument doc;
    if (doc.setContent(&fp)) {
      out = doc;
    }
    fp.close();
  }
  return out;
}

static void saveSources(const QDomDocument &doc, const QString id = "0") {
  QString file("abebooks_"+id+".xml");
  QString xml(QDir::homePath());
  xml.append("/.cache/");
  xml.append(file);
  QFile fp(xml);
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream in(&fp);
    in.setCodec(QTextCodec::codecForName("ISO 8859-1"));
    in << doc.toString(1);
    fp.close();
  };
}

static void writeLogFile(const QDomDocument &doc) {
  QString xml(QDir::homePath());
  xml.append("/.cache/AbeBooks_OrderResponse.xml");
  QFile fp(xml);
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream in(&fp);
    in << doc.toByteArray(1);
    fp.close();
  }
}
#endif
#endif

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/abebooks"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "AbeBooks"
#endif

#ifndef Q_WS_WIN
#ifndef PLUGIN_ABEBOOKS_DEBUG
#define PLUGIN_ABEBOOKS_DEBUG false
#endif
#endif

struct ANTIQUACORE_EXPORT AbeBooksAccess {
  QString user;
  QString key;
};
Q_DECLARE_METATYPE(AbeBooksAccess);

#endif // ABEBOOKS_PLUGIN_CONFIG_H
