// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configpgsql.h"

#include <QGridLayout>
#include <QMetaType>
#include <QIntValidator>

ConfigPgSQL::ConfigPgSQL(QWidget *parent) : QWidget{parent} {

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  QLabel *title = new QLabel(this);
  title->setText("<h3>" + tr("Database Configuration") + "</h3>");
  layout->addWidget(title, row++, 0, 1, 2);

  QLabel *t_profile = new QLabel(tr("Profile:"), this);
  t_profile->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_profile, row, 0, 1, 1);

  m_profile = new QLineEdit(this);
  m_profile->setText("Default");
  m_profile->setObjectName("database_profile");
  layout->addWidget(m_profile, row++, 1, 1, 1);

  QLabel *t_hostname = new QLabel(tr("Address:"), this);
  t_hostname->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_hostname, row, 0, 1, 1);

  hostname = new QLineEdit(this);
  hostname->setObjectName("pg_hostname");
  hostname->setPlaceholderText("192.168.178.2");
  layout->addWidget(hostname, row++, 1, 1, 1);

  QLabel *t_dbname = new QLabel(tr("Database:"), this);
  t_dbname->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_dbname, row, 0, 1, 1);

  database = new QLineEdit(this);
  database->setObjectName("pg_database");
  database->setPlaceholderText("antiqua");
  layout->addWidget(database, row++, 1, 1, 1);

  QLabel *t_port = new QLabel(tr("Port:"), this);
  t_port->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_port, row, 0, 1, 1);

  port = new QLineEdit(this);
  port->setObjectName("pg_port");
  port->setPlaceholderText("5432");
  layout->addWidget(port, row++, 1, 1, 1);

  QIntValidator *validator = new QIntValidator(port);
  validator->setRange(1024, 49151);
  port->setValidator(validator);

  QLabel *t_username = new QLabel(tr("Username:"), this);
  t_username->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_username, row, 0, 1, 1);

  username = new QLineEdit(this);
  username->setObjectName("pg_username");
  username->setPlaceholderText("antiqua");
  layout->addWidget(username, row++, 1, 1, 1);

  QLabel *t_passwort = new QLabel(tr("Password:"), this);
  t_passwort->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(t_passwort, row, 0, 1, 1);

  password = new QLineEdit(this);
  password->setObjectName("pg_passwort");
  password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  layout->addWidget(password, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  setLayout(layout);
}

void ConfigPgSQL::setData(const QPair<QString, QVariant> &data) {
  QString name = data.first;
  QVariant value = data.second;
  if (name.isEmpty() || value.isNull())
    return;

  QLineEdit *e = findChild<QLineEdit *>(name);
  if (e != nullptr && value.metaType().id() == QMetaType::QString) {
    e->setText(value.toString().trimmed());
  }
}

const QString ConfigPgSQL::profile() {
  if (m_profile->text().isEmpty())
    return QString("Default");
  else
    return m_profile->text();
}

const QMap<QString, QString> ConfigPgSQL::configuration() {
  QMap<QString, QString> m;
  m.insert("database_profile", profile());
  m.insert("pg_hostname", hostname->text().trimmed());
  m.insert("pg_database", database->text().trimmed());
  m.insert("pg_port", port->text().trimmed());
  m.insert("pg_username", username->text().trimmed());
  m.insert("pg_password", password->text().trimmed());
  return m;
}
