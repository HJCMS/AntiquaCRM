// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef CONFIGPGSQL_H
#define CONFIGPGSQL_H

#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPair>
#include <QString>

class ConfigPgSQL : public QWidget {
  Q_OBJECT

private:
  QLineEdit *m_profile;
  QLineEdit *hostname;
  QLineEdit *database;
  QLineEdit *port;
  QLineEdit *username;
  QLineEdit *password;

public:
  explicit ConfigPgSQL(QWidget *parent = nullptr);
  void setData(const QPair<QString,QVariant> &);
  const QString profile();
  const QMap<QString,QString> configuration();
};

#endif // CONFIGPGSQL_H
