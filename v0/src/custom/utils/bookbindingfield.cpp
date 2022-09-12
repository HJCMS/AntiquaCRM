// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookbindingfield.h"

#include <QHBoxLayout>

#include <SqlCore>

BookBindingBox::BookBindingBox(QWidget *parent) : QComboBox{parent} {
  QString sql("SELECT bb_id,bb_description");
  sql.append(" FROM ref_book_binding");
  sql.append(" WHERE bb_id>0 ORDER BY bb_id;");
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    insertItem(0, tr("Without disclosures"), 0);
    int r = 1;
    while (q.next()) {
      int id = q.value("bb_id").toInt();
      QString txt = q.value("bb_description").toString();
      insertItem(r++, txt, id);
    }
  }
}

BookBindingField::BookBindingField(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  m_box = new BookBindingBox(this);
  m_box->setEditable(false);
  m_box->setToolTip(tr("Book Condition"));
  layout->addWidget(m_box);
  setLayout(layout);
  setModified(false);
  setRequired(false);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void BookBindingField::dataChanged(int) { setModified(true); }

void BookBindingField::setValue(const QVariant &val) {
  int id = val.toInt();
  if (id > 0) {
    int index = m_box->findData(id, Qt::UserRole, Qt::MatchExactly);
    m_box->setCurrentIndex(index);
  }
}

void BookBindingField::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void BookBindingField::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void BookBindingField::setInfo(const QString &info) { m_box->setToolTip(info); }

const QString BookBindingField::info() { return m_box->toolTip(); }

const QVariant BookBindingField::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toInt();
}

bool BookBindingField::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

const QString BookBindingField::notes() {
  return tr("Missing required Book Binding!");
}
