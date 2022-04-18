// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SETTINGS_H
#define ANTIQUACRM_SETTINGS_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

class ApplSettings : public QSettings {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QHash<QString, QVariant> p_hash;

public:
  explicit ApplSettings(QObject *parent = nullptr);
  static const QString sqlConnectioName();
  void writeGroupConfig(const QString &, const QHash<QString, QVariant> &);
  const QHash<QString, QVariant> &readGroupConfig(const QString &);
  virtual ~ApplSettings();
};

#endif // ANTIQUACRM_SETTINGS_H
