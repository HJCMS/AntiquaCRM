// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationedit.h"

#include <QDebug>
#include <QVBoxLayout>

DesignationEdit::DesignationEdit(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(2, 0, 2, 2);

  m_keyword = new QLineEdit(this);
  m_keyword->setToolTip(tr("Keywords"));
  layout->addWidget(m_keyword);

  m_description = new QTextEdit(this);
  m_description->setToolTip(tr("Description"));
  layout->addWidget(m_description);

  m_statusBar = new QStatusBar(this);
  btn_save = new QPushButton(QIcon("://icons/action_save.png"), tr("Save"),
                             m_statusBar);
  m_statusBar->addPermanentWidget(btn_save);
  layout->addWidget(m_statusBar);
  layout->setStretch(1, 1);
  setLayout(layout);
  connect(btn_save, SIGNAL(clicked()), SLOT(setSave()));
}

void DesignationEdit::setSave() {
  QString set;
  QString buffer = m_keyword->text().trimmed();
  if(buffer.isEmpty()) {
    m_statusBar->showMessage(tr("Empty Book designation will not accepted!"));
    return;
  }

  if (origin_t.keyword != buffer) {
    buffer.replace("'", "’");
    set.append("bd_keyword='" + buffer + "'");
  }
  buffer.clear();
  buffer = m_description->toPlainText().trimmed();
  if (origin_t.description != buffer) {
    buffer.replace("'", "’");
    set.append("bd_description='" + buffer + "'");
  }

  if (set.isEmpty()) {
    m_statusBar->showMessage(tr("Nothing to save"));
    return;
  }

  QString sql("UPDATE ref_book_designation SET ");
  sql.append(set);
  sql.append(" WHERE " + origin_t.whereclause);
  sql.append(set);

  emit sendSaveQuery(sql);
}

void DesignationEdit::setClear() {
  m_description->clear();
  m_keyword->clear();
  m_keyword->setFocus();
}

void DesignationEdit::setMessage(const QString &msg) {
  m_statusBar->showMessage(msg);
}

void DesignationEdit::setEditItem(const DesignationData &data) {
  setClear();
  origin_t = data;
  m_keyword->setText(data.keyword);
  m_description->setPlainText(data.description);
}
