// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef APPLSETTINGS_ANTIQUACORE_H
#define APPLSETTINGS_ANTIQUACORE_H

#include "antiqua_global.h"

#include <QHash>
#include <QDir>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

class ANTIQUACORE_EXPORT ApplSettings : public QSettings {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QHash<QString, QVariant> p_hash;

public:
  explicit ApplSettings(QObject *parent = nullptr);
  static const QString sqlConnectioName();
  bool needAssistant();
  void writeGroupConfig(const QString &, const QHash<QString, QVariant> &);
  const QHash<QString, QVariant> &readGroupConfig(const QString &);
  static const QDir getDataTarget();
  virtual ~ApplSettings();
};

#endif // APPLSETTINGS_ANTIQUACORE_H
