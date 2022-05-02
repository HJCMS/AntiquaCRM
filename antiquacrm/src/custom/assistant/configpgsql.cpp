// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configpgsql.h"

#include <QGridLayout>

ConfigPgSQL::ConfigPgSQL(QWidget *parent) : QWidget{parent} {

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  QLabel *title = new QLabel(this);
  title->setText("<h3>" + tr("Database Configuration") + "</h3>");
  layout->addWidget(title, row++, 0, 1, 2);

  QLabel *t_hostname = new QLabel(tr("Address:"), this);
  t_hostname->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_hostname, row, 0, 1, 1);

  hostname = new QLineEdit(this);
  hostname->setObjectName("hostname");
  hostname->setPlaceholderText("192.168.178.2");
  layout->addWidget(hostname, row++, 1, 1, 1);

  QLabel *t_dbname = new QLabel(tr("Database:"), this);
  t_dbname->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_dbname, row, 0, 1, 1);

  dbname = new QLineEdit(this);
  dbname->setObjectName("databasename");
  dbname->setPlaceholderText("antiquacrm");
  layout->addWidget(dbname, row++, 1, 1, 1);

  QLabel *t_port = new QLabel(tr("Port:"), this);
  t_port->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_port, row, 0, 1, 1);

  port = new QLineEdit(this);
  port->setObjectName("port");
  port->setPlaceholderText("5432");
  layout->addWidget(port, row++, 1, 1, 1);

  QLabel *t_username = new QLabel(tr("Username:"), this);
  t_username->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_username, row, 0, 1, 1);

  username = new QLineEdit(this);
  username->setObjectName("username");
  layout->addWidget(username, row++, 1, 1, 1);

  QLabel *t_passwort = new QLabel(tr("Password:"), this);
  t_passwort->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_passwort, row, 0, 1, 1);

  password = new QLineEdit(this);
  password->setObjectName("passwort");
  password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  layout->addWidget(password, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  setLayout(layout);
}

const QMap<QString, QString> ConfigPgSQL::configuration() {
  QMap<QString, QString> m;
  m.insert("hostname", hostname->text().trimmed());
  m.insert("database", dbname->text().trimmed());
  m.insert("port", port->text().trimmed());
  m.insert("username", username->text().trimmed());
  m.insert("password", password->text().trimmed());
  return m;
}
