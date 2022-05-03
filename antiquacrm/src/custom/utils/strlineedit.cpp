// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "strlineedit.h"

#include <QDebug>
#include <QRegExp>
#include <QRegularExpression>
#include <QHBoxLayout>

StrLineEdit::StrLineEdit(QWidget *parent)
    : UtilsMain{parent}, p_table("ui_autofill_keywords") {
  if (objectName().isEmpty())
    setObjectName("StrLineEdit");

  m_sql = new HJCMS::SqlCore(this);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_lineEdit = new QLineEdit(this);
  layout->addWidget(m_lineEdit);

  QRegExp reg(regPattern);
  m_validator = new QRegExpValidator(reg, this);
  m_lineEdit->setValidator(m_validator);

  setLayout(layout);
  setRequired(false);
  setModified(false);

  connect(m_lineEdit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void StrLineEdit::setValue(const QVariant &str) {
  if (str.toString().isEmpty())
    return;

  QRegExp reg("[\\n\\r]+");
  QString data = str.toString().trimmed();
  data = data.replace(reg, "");
  reg.setPattern("\\'");
  data = data.replace(reg, "´");

  QRegularExpression regexp(regPattern);
  QRegularExpressionMatch match = regexp.match(data);
  if (match.hasMatch()) {
    m_lineEdit->setText(data);
    setModified(true);
    return;
  }
  qDebug() << Q_FUNC_INFO << "Invalid:" << str;
}

void StrLineEdit::reset() {
  m_lineEdit->clear();
  setModified(false);
}

void StrLineEdit::setFocus() { m_lineEdit->setFocus(); }

const QVariant StrLineEdit::value() {
  QRegExp reg("[\\n\\r]+");
  QString buffer(m_lineEdit->text().trimmed());
  buffer = buffer.replace(reg, "");
  return QVariant(buffer);
}

void StrLineEdit::setLineEditCompliter(const QStringList &list) {
  if (list.size() < 1)
    return;

  m_completer = new QCompleter(list, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setFilterMode(Qt::MatchContains);
  m_lineEdit->setCompleter(m_completer);
}

void StrLineEdit::inputChanged(const QString &str) {
  if (str.length() >= (m_lineEdit->maxLength() - 1)) {
    m_lineEdit->setStyleSheet("color: red;");
  } else {
    m_lineEdit->setStyleSheet("");
  }
  setModified(true);
}

void StrLineEdit::skipReturnPressed() { setModified(true); }

void StrLineEdit::setTableName(const QString &table) { p_table = table; }

const QString StrLineEdit::tableName() { return p_table; }

void StrLineEdit::loadStorageKeywords() {
  // Nur wenn Autovervollständigen an ist, einschalten!
  m_lineEdit->setClearButtonEnabled(true);

  QString select("SELECT DISTINCT c_name FROM extern_categories");
  select.append(" WHERE c_name IS NOT NULL ORDER BY c_name ASC;");

  QStringList list;
  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    while (q.next()) {
      if (q.value(0).isValid())
        list << q.value(0).toString();
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }

  if (list.size() > 1)
    setLineEditCompliter(list);
}

void StrLineEdit::loadDataset(const QString &key) {
  if (key.isEmpty())
    return;

  // Nur wenn Autovervollständigen an ist, einschalten!
  m_lineEdit->setClearButtonEnabled(true);

  QString select("SELECT DISTINCT k_keyword FROM ");
  select.append(tableName());
  select.append(" WHERE k_table_cell LIKE '");
  select.append(key);
  select.append("';");

  QStringList list;
  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    while (q.next()) {
      if (q.value(0).isValid())
        list << q.value(0).toString();
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }

  if (list.size() > 1)
    setLineEditCompliter(list);
}

void StrLineEdit::setMaxAllowedLength(int l) {
  // Standard Wert ist 32767.
  if (l > 4 && l < 32767)
    m_lineEdit->setMaxLength(l);

  QString txt(tr("Max allowed length"));
  txt.append(" ");
  txt.append(QString::number(l));
  m_lineEdit->setPlaceholderText(txt);
}

bool StrLineEdit::isValid() {
  if (isRequired() && m_lineEdit->text().isEmpty()) {
    return false;
  } else if (m_lineEdit->text().length() > m_lineEdit->maxLength()) {
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
  if (m_lineEdit->text().isEmpty()) {
    msg.append(tr("is required and can not empty."));
    return msg;
  } else if (m_lineEdit->text().length() > m_lineEdit->maxLength()) {
    msg.append(tr("invalid length."));
    return msg;
  } else {
    msg.append(tr("contains invalid input."));
    return msg;
  }
}
