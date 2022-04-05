// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "strlineedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

StrLineEdit::StrLineEdit(QWidget *parent) : QLineEdit{parent} {
  setObjectName("StrLineEdit");

  QRegExp pattern("([\\w\\d\\s\\-\\+,\\.\\?\\!`´:;&#]+)");
  m_validator = new QRegExpValidator(pattern, this);
  setValidator(m_validator);
  connect(this, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));
}

void StrLineEdit::setLineEditCompliter(const QStringList &list) {
  if (list.size() < 1)
    return;

  m_completer = new QCompleter(list, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  setCompleter(m_completer);
}

void StrLineEdit::inputChanged(const QString &s) {
  if (s.length() >= (maxLength()-1)) {
    setStyleSheet("color: red;");
  } else {
    setStyleSheet("");
  }
}

void StrLineEdit::setKeyword(const QString &key) {
  if (key.isEmpty())
    return;

  // Nur wenn Autovervollständigen an ist, einschalten!
  setClearButtonEnabled(true);

  QString select("SELECT DISTINCT k_keyword FROM ");
  select.append(tableName());
  select.append(" WHERE k_table_cell LIKE '");
  select.append(key);
  select.append("';");

  QStringList list;
  QSqlDatabase db(QSqlDatabase::database(sqlConnectionName));
  // qDebug() << select << db.isValid();
  if (db.isValid()) {
    QSqlQuery q = db.exec(select);
    while (q.next()) {
      if (q.value(0).isValid())
        list << q.value(0).toString();
    }
  }
  setLineEditCompliter(list);
}

void StrLineEdit::setMaxAllowedLength(int l)
{
    // Standard Wert ist 32767.
    if(l > 4 && l < 32767)
        setMaxLength(l);

    QString txt(tr("Max allowed length"));
    txt.append(" ");
    txt.append(QString::number(l));
    setPlaceholderText(txt);
}
