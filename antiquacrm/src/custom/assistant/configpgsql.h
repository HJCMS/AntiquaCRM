// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CONFIGPGSQL_H
#define CONFIGPGSQL_H

#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>
#include <QMap>
#include <QPair>
#include <QString>

class ConfigPgSQL : public QWidget {
  Q_OBJECT

private:
  QLineEdit *hostname;
  QLineEdit *database;
  QLineEdit *port;
  QLineEdit *username;
  QLineEdit *password;

public:
  explicit ConfigPgSQL(QWidget *parent = nullptr);
  void setData(const QPair<QString,QVariant> &);
  const QMap<QString,QString> configuration();
};

#endif // CONFIGPGSQL_H
