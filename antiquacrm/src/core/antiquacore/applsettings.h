// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef APPLSETTINGS_ANTIQUACORE_H
#define APPLSETTINGS_ANTIQUACORE_H

#include <QHash>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

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

#endif // APPLSETTINGS_ANTIQUACORE_H
