// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKER_PLUGIN_CONFIG_H
#define BOOKLOOKER_PLUGIN_CONFIG_H

#include <AntiquaInterface>
#include <QByteArray>
#include <QFile>
#include <QJsonDocument>
#include <QMetaType>
#include <QString>

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd"
#endif

/** @brief Konfigurationsgruppe */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/booklooker"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "Booklooker"
#endif

#ifndef BOOKLOOKER_API_VERSION
#define BOOKLOOKER_API_VERSION "2.0"
#endif

#ifndef BOOKLOOKER_AUTH_PATH
#define BOOKLOOKER_AUTH_PATH "/2.0/authenticate"
#endif

#ifndef BOOKLOOKER_TOKEN_ENV
#define BOOKLOOKER_TOKEN_ENV "BOOKLOOKER_TOKEN"
#endif

#ifdef ANTIQUA_DEVELOPEMENT
#include <QJsonDocument>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

static QJsonDocument getSource(const QString &id) {
  QString file(QDir::homePath());
  file.append("/.cache/");
  file.append("booklooker_" + id + ".json");

  QFile fp(file);
  if (fp.open(QIODevice::ReadOnly)) {
    QString buffer;
    QTextStream js(&fp);
    buffer = js.readAll();
    fp.close();
    QJsonDocument doc = QJsonDocument::fromJson(buffer.toLocal8Bit());
    buffer.clear();
    return doc;
  }
  return QJsonDocument();
}

static void saveSources(const QJsonDocument &doc, const QString &id) {
  QString file(QDir::homePath());
  file.append("/.cache/");
  file.append("booklooker_" + id + ".json");

  QFile fp(file);
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream in(&fp);
    // in.setCodec(QTextCodec::codecForName("ISO 8859-1"));
    in << doc.toJson(QJsonDocument::Indented);
    fp.close();
  };
}

#endif

struct ANTIQUACORE_EXPORT BooklookerAccess {
  QString user;
  QString key;
};
Q_DECLARE_METATYPE(BooklookerAccess);

#endif // BOOKLOOKER_PLUGIN_CONFIG_H
