// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SHAREDDATAFILES_H
#define ANTIQUACRM_SHAREDDATAFILES_H

#include <QDir>
#include <QDomDocument>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <AGlobal>
#include <ASettings>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY ASharedDataFiles : public QDir {
public:
  explicit ASharedDataFiles(const QDir &d = ASettings::getUserDataDir());
  const QStringList dataFiles();
  static const QStringList defaultFilter();
  bool fileExists(const QString &basename,
                  const QStringList &ext = defaultFilter());
  bool storeJson(const QString &basename, const QJsonDocument &doc);
  const QJsonDocument getJson(const QString &basename);
  bool storeXml(const QString &basename, const QDomDocument &xml);
  const QDomDocument getXML(const QString &basename);
};

class ANTIQUACRM_LIBRARY ASharedCacheFiles : public QDir {
public:
  explicit ASharedCacheFiles();
  static const QString cacheDir();
  bool storeTempFile(const QString &filename, const QByteArray &data);
  bool storeTempFile(const QString &filename, const QString &data);
  const QString getTempFile(const QString &filename);
  const QJsonObject getTempJson(const QString &md5sum);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SHAREDDATAFILES_H
