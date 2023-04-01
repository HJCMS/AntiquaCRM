// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ashareddatafiles.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QTextStream>

namespace AntiquaCRM {

ASharedDataFiles::ASharedDataFiles(const QDir &d) : QDir{d} {
  setNameFilters(defaultFilter());
}

const QStringList ASharedDataFiles::dataFiles() {
  return entryList((QDir::Files | QDir::Writable), QDir::Name);
}

const QStringList ASharedDataFiles::defaultFilter() {
  return QStringList({"*.xml", "*.sql", "*.json"});
}

bool ASharedDataFiles::needsUpdate(const QString &basename,
                                   const QStringList &ext) {
  if (ext.count() > 0)
    setNameFilters(ext);

  bool status = false;
  QDateTime dt;
  QFileInfoList li = entryInfoList((QDir::Files | QDir::Writable), QDir::Name);
  foreach (QFileInfo i, li) {
    if (i.baseName() == basename) {
      status = true;
      dt = i.fileTime(QFileDevice::FileMetadataChangeTime);
      break;
    }
  }
  setNameFilters(defaultFilter());

  if (status)
    return (dt.date() == QDate::currentDate()) ? false : true;

  return true;
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
#ifdef ANTIQUA_DEVELOPEMENT
    data << doc.toJson(QJsonDocument::Indented);
#else
    data << doc.toJson(QJsonDocument::Compact);
#endif
    fp.close();
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
  qWarning("Can't save '%s' Json!", qPrintable(info.fileName()));
#endif
  return false;
}

const QJsonDocument ASharedDataFiles::getJson(const QString &basename) {
  QJsonDocument doc;
  QJsonParseError parseHandle;
  QFileInfo info(path(), basename + ".json");
  if (!info.isReadable()) {
    qWarning("No File or Permission denied (%s).", qPrintable(info.filePath()));
    return doc;
  }
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    QByteArray buffer = data.readAll().toLocal8Bit();
    doc = QJsonDocument::fromJson(buffer, &parseHandle);
    if (parseHandle.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.",
               qPrintable(parseHandle.errorString()));
      doc = QJsonDocument();
    }
    fp.close();
    buffer.clear();
  }
  return doc;
}

bool ASharedDataFiles::storeXml(const QString &basename,
                                const QDomDocument &xml) {
  QFileInfo info(path(), basename + ".xml");
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream data(&fp);
#ifdef ANTIQUA_DEVELOPEMENT
    // Indented: 1
    data << xml.toString(1);
#else
    // Compat: -1
    data << xml.toString(-1);
#endif
    fp.close();
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
  qWarning("Can't save '%s' XML!", qPrintable(info.fileName()));
#endif
  return false;
}

const QDomDocument ASharedDataFiles::getXML(const QString &basename) {
  QDomDocument doc;
  QFileInfo info(path(), basename + ".xml");
  if (!info.isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
    qWarning("Can't load '%s' XML!", qPrintable(info.fileName()));
#endif
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

const QStringList ASharedDataFiles::getCompleterList(const QString &basename,
                                                     const QString &key) {
  QStringList list;
  QJsonDocument doc = getJson(basename);
  if (doc.isEmpty()) {
    qWarning("ACompleterData::getCompleterList missing '%s'",
             qPrintable(basename));
    return list;
  }
  QJsonArray arr = doc.object().value(basename).toArray();
  for (int i = 0; i < arr.count(); i++) {
    QJsonObject obj = arr[i].toObject();
    list << obj.value(key).toString();
  }
  return list;
}

ASharedCacheFiles::ASharedCacheFiles(const QDir &d) : QDir{d} {}

bool ASharedCacheFiles::storeTempFile(const QString &filename,
                                      const QByteArray &data) {
  QFileInfo info(path(), filename);
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream stream(&fp);
    stream << QString::fromLocal8Bit(data);
    fp.close();
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
  qWarning("Can't save '%s' Tempfile", qPrintable(info.fileName()));
#endif
  return false;
}

bool ASharedCacheFiles::storeTempFile(const QString &filename,
                                      const QString &data) {
  QFileInfo info(path(), filename);
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream stream(&fp);
    stream << data;
    fp.close();
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
  qWarning("Can't save '%s' Tempfile", qPrintable(info.fileName()));
#endif
  return false;
}

const QString ASharedCacheFiles::getTempFile(const QString &filename) {
  QFileInfo info(path(), filename);
  if (!info.isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
    qWarning("Temp file: %s not exists", qPrintable(info.fileName()));
#endif
    return QString();
  }
  QString buffer;
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    buffer = data.readAll();
    fp.close();
  }
  return buffer;
}

const QJsonObject ASharedCacheFiles::getTempJson(const QString &md5sum) {
  QFileInfo info(path(), md5sum + ".json");
  if (!info.isReadable()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Permissions:" << info;
#else
    qWarning("Temp file: %s not exists", qPrintable(info.fileName()));
#endif
    return QJsonObject();
  }

  QJsonDocument doc;
  QJsonParseError parseHandle;
  QFile fp(info.filePath());
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream data(&fp);
    QByteArray buffer = data.readAll().toLocal8Bit();
    doc = QJsonDocument::fromJson(buffer, &parseHandle);
    if (parseHandle.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.",
               qPrintable(parseHandle.errorString()));
      doc = QJsonDocument();
    }
    fp.close();
    buffer.clear();
  }
  return doc.object();
}

}; // namespace AntiquaCRM
