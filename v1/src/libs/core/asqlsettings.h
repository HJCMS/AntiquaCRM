// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLSETTINGS_H
#define ANTIQUACRM_SQLSETTINGS_H

#include <QObject>
#include <QSettings>

namespace AntiquaCRM {

class ASqlSettings : public QSettings {
  Q_OBJECT
  Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY
                 sendProfileChanged)

private:
  QString profile = QString("Default");

Q_SIGNALS:
  void sendProfileChanged();

public:
  explicit ASqlSettings(QObject *parent = nullptr);

  static const QString configDomain();
  static const QString connectionName();

  void setProfile(const QString &name);

  const QString getProfile();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLSETTINGS_H
