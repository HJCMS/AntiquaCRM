// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationedit.h"

#include <QDebug>
#include <QGridLayout>

DesignationEdit::DesignationEdit(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  int grow = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 0, 2, 0);

  layout->addWidget(label(tr("Keyword")), grow, 0, 1, 1);
  m_keyword = new QLineEdit(this);
  m_keyword->setToolTip(tr("Book cover description"));
  m_keyword->setPlaceholderText(m_keyword->toolTip());
  layout->addWidget(m_keyword, grow++, 1, 1, 1);

  layout->addWidget(label(tr("Description")), grow, 0, 1, 1);
  m_id = new QSpinBox(this);
  m_id->setRange(0, 9999);
  m_id->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_id->setValue(0);
  m_id->setPrefix(tr("Database Id") + " ");
  m_id->setEnabled(false);
  m_id->setStyleSheet("QSpinBox {background:transparent; border:none;}");
  layout->addWidget(m_id, grow++, 1, 1, 1, Qt::AlignLeft);

  m_description = new QTextEdit(this);
  m_description->setToolTip(tr("Description for this entry."));
  layout->addWidget(m_description, grow++, 0, 1, 2);

  setLayout(layout);
}

QLabel *DesignationEdit::label(const QString &txt) {
  QLabel *lb = new QLabel(txt, this);
  return lb;
}

const QRegularExpression DesignationEdit::spaces() const {
  return QRegularExpression("(\\s{2,}|[\\t\\n\\r]+)");
}

const QString DesignationEdit::getKeyword() {
  QString out = m_keyword->text();
  out.replace("'", "’");
  out.replace(spaces(), " ");
  return out.trimmed();
}

const QString DesignationEdit::getDesciption() {
  QString out = m_description->toPlainText();
  out.replace("'", "’");
  out.replace(spaces(), " ");
  return out.trimmed();
}

void DesignationEdit::createSqlInsert() {
  QString values;
  QString buffer = getKeyword();
  values.append("'" + buffer + "'");
  buffer.clear();
  buffer = getDesciption();
  if (!buffer.isEmpty()) {
    values.append(",'" + buffer + "'");
    buffer.clear();
  }

  QString sql("INSERT INTO ref_book_designation");
  sql.append(" (bd_keyword,bd_description)");
  sql.append(" VALUES (" + values + ");");

  emit sendSaveQuery(sql);
}

void DesignationEdit::createSqlUpdate() {
  int id = m_id->value();
  if (id < 1)
    return;

  QString set;
  QString buffer = getKeyword();
  if (origin_t.keyword != buffer) {
    buffer.replace("'", "’");
    set.append("bd_keyword='" + buffer + "'");
  }
  buffer.clear();

  buffer = getDesciption();
  if (origin_t.description != buffer) {
    buffer.replace("'", "’");
    set.append("bd_description='" + buffer + "'");
  }
  buffer.clear();

  if (set.isEmpty()) {
    emit sendStatusMessage(tr("Nothing to save"));
    return;
  }

  QString sql("UPDATE ref_book_designation SET ");
  sql.append(set);
  sql.append(" WHERE bd_id=");
  sql.append(QString::number(id));
  sql.append(";");

  emit sendSaveQuery(sql);
}

void DesignationEdit::setClear() {
  m_id->clear();
  m_description->clear();
  m_keyword->clear();
}

void DesignationEdit::setCreateNew() {
  setClear();
  m_id->setValue(0);
  m_keyword->setFocus();
}

void DesignationEdit::setDelete() {
  int id = m_id->value();
  if (id < 1) {
    emit sendStatusMessage(tr("No Entry selected, delete canceled!"));
    return;
  }

  QString sql("DELETE FROM ref_book_designation");
  sql.append(" WHERE bd_id=");
  sql.append(QString::number(id));
  sql.append(";");

  emit sendSaveQuery(sql);
}

void DesignationEdit::setSave() {
  int id = m_id->value();
  if (getKeyword().length() < 5) {
    emit sendStatusMessage(tr("Empty Book designation will not accepted!"));
    return;
  }

  if (id == 0) {
    createSqlInsert();
  } else if (id > 0 && id != origin_t.id) {
    emit sendStatusMessage(tr("Database Indexes mismatch, update canceled!"));
    return;
  }
  createSqlUpdate();
}

void DesignationEdit::setEditItem(const DesignationData &data) {
  origin_t = data;
  m_id->setValue(data.id);
  m_keyword->setText(data.keyword);
  m_description->setPlainText(data.description);
}
