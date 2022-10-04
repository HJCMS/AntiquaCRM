// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ashareddatafiles.h"
#include "aglobal.h"
#include "asettings.h"

#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QJsonParseError>
#include <QStandardPaths>
#include <QTextStream>

namespace AntiquaCRM {

ASharedDataFiles::ASharedDataFiles() : QDir(ASettings::getUserDataDir()) {
  setNameFilters(defaultFilter());
}

const QStringList ASharedDataFiles::dataFiles() {
  return entryList((QDir::Files | QDir::Writable), QDir::Name);
}

const QStringList ASharedDataFiles::defaultFilter() {
  return QStringList({"*.xml", "*.sql", "*.json"});
}

bool ASharedDataFiles::fileExists(const QString &basename,
                                  const QStringList &ext) {
  if (ext.count() > 0)
    setNameFilters(ext);

  bool status = false;
  QFileInfoList li = entryInfoList((QDir::Files | QDir::Writable), QDir::Name);
  foreach (QFileInfo i, li) {
    if (i.baseName() == basename) {
      status = true;
      break;
    }
  }
  setNameFilters(defaultFilter());
  return status;
}

bool ASharedDataFiles::storeJson(const QString &basename,
                                 const QJsonDocument &doc) {
  QFileInfo info(path(), basename + ".json");
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    data << doc.toJson(QJsonDocument::Compact); //Indented
    fp.close();
    return true;
  }
  qWarning("Permission Denied!");
  return false;
}

const QJsonDocument ASharedDataFiles::getJson(const QString &basename) {
  QJsonDocument doc;
  QJsonParseError parseHandle;
  QFileInfo info(path(), basename + ".json");
  if (!info.isReadable()) {
    qWarning("Permission Denied!");
    return doc;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    QByteArray buffer = data.readAll().toLocal8Bit();
    doc.fromJson(buffer, &parseHandle);
    if (parseHandle.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.",
               qPrintable(parseHandle.errorString()));
      doc = QJsonDocument();
    }
    buffer.clear();
    fp.close();
  }
  return doc;
}

bool ASharedDataFiles::storeXml(const QString &basename,
                                const QDomDocument &xml) {
  QFileInfo info(path(), basename + ".xml");
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    data << xml.toString(-1);
    fp.close();
    return true;
  }
  qWarning("Permission Denied!");
  return false;
}

const QDomDocument ASharedDataFiles::getXML(const QString &basename) {
  QDomDocument doc;
  QFileInfo info(path(), basename + ".xml");
  if (!info.isReadable()) {
    qWarning("Permission Denied!");
    return doc;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QString errMsg;
    if (!doc.setContent(&fp, false, &errMsg)) {
      qWarning("XML Errors: '%s'.", qPrintable(errMsg));
    }
    fp.close();
  }
  return doc;
}

ASharedCacheFiles::ASharedCacheFiles() : QDir(cacheDir()) {}

const QString ASharedCacheFiles::cacheDir() {
  return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

bool ASharedCacheFiles::storeTempFile(const QString &filename,
                                      const QByteArray &data) {
  QFileInfo info(path(), filename);
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QDataStream stream(&fp);
    stream << data;
    fp.close();
    return true;
  }
  qWarning("Permission Denied!");
  return false;
}

bool ASharedCacheFiles::storeTempFile(const QString &filename,
                                      const QString &data) {
  QFileInfo info(path(), filename);
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(ANTIQUACRM_TEXTCODEC);
    stream << data;
    fp.close();
    return true;
  }
  qWarning("Permission Denied!");
  return false;
}

const QString ASharedCacheFiles::getTempFile(const QString &filename) {
  QFileInfo info(path(), filename);
  if (!info.isReadable()) {
    qWarning("Permission Denied!");
    return QString();
  }
  QString buffer;
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    data.setCodec(ANTIQUACRM_TEXTCODEC);
    buffer = data.readAll();
    fp.close();
  }
  return buffer;
}

}; // namespace AntiquaCRM
