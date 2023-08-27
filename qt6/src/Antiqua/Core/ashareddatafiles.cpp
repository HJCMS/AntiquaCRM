// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ashareddatafiles.h"

#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLocale>
#include <QTextStream>

namespace AntiquaCRM {

ASharedDataFiles::ASharedDataFiles(const QDir &d) : QDir{d} {
  setNameFilters(defaultFilter());
}

const QString ASharedDataFiles::bcp47Suffix() const {
  QString _str("_");
  _str.append(QLocale::system().bcp47Name().toLower());
  return _str;
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
  QJsonDocument _doc;
  QFileInfo _info(path(), basename + bcp47Suffix() + ".json");
  if (!_info.isReadable())
    _info.setFile(path(), basename + ".json");

  if (!_info.isReadable()) {
    qWarning("Missing Json Document or Permission denied for (%s).",
             qPrintable(_info.filePath()));
    return _doc;
  }

  QFile _fp(_info.filePath());
  if (_fp.open(QIODevice::ReadOnly)) {
    QTextStream _stream(&_fp);
    QByteArray _jsdata = _stream.readAll().toLocal8Bit();
    QJsonParseError _parser;
    _doc = QJsonDocument::fromJson(_jsdata, &_parser);
    if (_parser.error != QJsonParseError::NoError) {
      qWarning("Json Document Error: '%s'.", qPrintable(_parser.errorString()));
      _doc = QJsonDocument();
    }
    _fp.close();
    _jsdata.clear();
  }
  return _doc;
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
  QDomDocument _doc;
  QFileInfo _info(path(), basename + bcp47Suffix() + ".xml");
  if (!_info.isReadable())
    _info.setFile(path(), basename + ".xml");

  if (!_info.isReadable()) {
    qWarning("Can't load '%s' XML!", qPrintable(_info.fileName()));
    return _doc;
  }

  QFile _fp(_info.filePath());
  if (_fp.open(QIODevice::ReadOnly)) {
    QString _errno;
    if (!_doc.setContent(&_fp, false, &_errno)) {
      qWarning("XML Errors: '%s'.", qPrintable(_errno));
      return QDomDocument();
    }
    _fp.close();
  }
  return _doc;
}

const QStringList ASharedDataFiles::getCompleterList(const QString &basename,
                                                     const QString &key) {
  QStringList _list;
  QJsonDocument _data = getJson(basename);
  if (_data.isEmpty() || _data.object().isEmpty()) {
    qWarning("ACompleterData::getCompleterList missing '%s'",
             qPrintable(basename));
    return _list;
  }

  QJsonArray _array = _data.object().value(basename).toArray();
  for (int i = 0; i < _array.count(); i++) {
    QJsonObject _obj = _array[i].toObject();
    _list << _obj.value(key).toString();
  }
  return _list;
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
