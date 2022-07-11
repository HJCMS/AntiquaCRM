// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_CONFIG_H
#define ABEBOOKS_PLUGIN_CONFIG_H

#include <AntiquaInterface>
#include <QByteArray>
#include <QDomDocument>
#include <QMetaType>
#include <QString>
#include <QTextStream>

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QTextCodec>

static const QDomDocument readSources(const QString &id) {
  QString file(QDir::homePath());
  file.append("/.cache/");
  file.append("abebooks_" + id + ".xml");

  QFile fp(file);
  QDomDocument doc;
  if (fp.open(QIODevice::ReadOnly)) {
    if (!doc.setContent(&fp))
      qWarning("Can't open XMLDocument %s", qPrintable(file));

    fp.close();
  }
  return doc;
}

static void saveSources(const QDomDocument &doc, const QString id = "0") {
  QString file(QDir::homePath());
  file.append("/.cache/");
  file.append("abebooks_" + id + ".xml");

  QFile fp(file);
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream in(&fp);
    in.setCodec(QTextCodec::codecForName("ISO 8859-1"));
    in << doc.toString(1);
    fp.close();
  };
}

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
