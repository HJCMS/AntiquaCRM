// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "strlineedit.h"
#include "applsettings.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

StrLineEdit::StrLineEdit(QWidget *parent) : QLineEdit{parent} {
  if (objectName().isEmpty())
    setObjectName("StrLineEdit");

  setWindowTitle(tr("String edit"));

  m_validator = new QRegExpValidator(pcre(), this);
  setValidator(m_validator);

  connect(this, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(this, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void StrLineEdit::setValue(const QVariant &str) {
  if (str.toString().isEmpty())
    return;

  QRegExp reg("[\\n\\r]+");
  QString data = str.toString().trimmed();
  data = data.replace(reg, "");
  reg.setPattern("\\'");
  data = data.replace(reg, "´");
  QRegExp reg2(pcre());
  if (data.contains(reg2)) {
    setText(data);
    setModified(true);
    return;
  }
  qDebug() << Q_FUNC_INFO << " INVALID DATA" << str;
}

void StrLineEdit::setModified(bool b) {
  modified = b;
  QLineEdit::setModified(b);
}

void StrLineEdit::reset() {
  clear();
  setModified(false);
}

void StrLineEdit::setRequired(bool b) { required = b; }

bool StrLineEdit::isRequired() { return required; }

bool StrLineEdit::hasModified() {
  return (isModified() || modified);
}

const QVariant StrLineEdit::value() {
  QRegExp reg("[\\n\\r]+");
  QString buffer(text().trimmed());
  buffer = buffer.replace(reg, "");
  return QVariant(buffer);
}

void StrLineEdit::setLineEditCompliter(const QStringList &list) {
  if (list.size() < 1)
    return;

  m_completer = new QCompleter(list, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setFilterMode(Qt::MatchContains);
  setCompleter(m_completer);
}

void StrLineEdit::inputChanged(const QString &str) {
  if (str.length() >= (maxLength() - 1)) {
    setStyleSheet("color: red;");
  } else {
    setStyleSheet("");
  }
  setModified(true);
}

void StrLineEdit::skipReturnPressed() { setModified(true); }

void StrLineEdit::loadDataset(const QString &key) {
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
  QSqlDatabase db(QSqlDatabase::database(ApplSettings::sqlConnectioName()));
  // qDebug() << select << db.isValid();
  if (db.isValid()) {
    QSqlQuery q = db.exec(select);
    while (q.next()) {
      if (q.value(0).isValid())
        list << q.value(0).toString();
    }
  } else {
    qWarning("%s: %s", qUtf8Printable(objectName()),
             qUtf8Printable(" - can not open Database."));
  }
  if (list.size() > 1)
    setLineEditCompliter(list);
}

void StrLineEdit::setMaxAllowedLength(int l) {
  // Standard Wert ist 32767.
  if (l > 4 && l < 32767)
    setMaxLength(l);

  QString txt(tr("Max allowed length"));
  txt.append(" ");
  txt.append(QString::number(l));
  setPlaceholderText(txt);
}

bool StrLineEdit::isValid() {
  if (required && text().isEmpty()) {
    return false;
  } else if (text().length() > maxLength()) {
    return false;
  }

  return true;
}

const QString StrLineEdit::notes() {
  QString msg(tr("The field"));
  if (windowTitle().isEmpty()) {
    msg.append(" " + objectName() + " ");
  } else {
    msg.append(" " + windowTitle() + " ");
  }
  if (text().isEmpty()) {
    msg.append(tr("is required and can not empty."));
    return msg;
  } else if (text().length() > maxLength()) {
    msg.append(tr("invalid length."));
    return msg;
  } else {
    msg.append(tr("contains invalid input."));
    return msg;
  }
}
