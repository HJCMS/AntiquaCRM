// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messageselecter.h"

#include <QDebug>
#include <QGridLayout>
#include <QLabel>

MessageCaller::MessageCaller() {
  p_caller = QString();
  p_title = QString();
  p_body = QString();
}

void MessageCaller::setCaller(const QString &txt) { p_caller = txt; }

const QString MessageCaller::getCaller() { return p_caller; }

void MessageCaller::setTitle(const QString &txt) { p_title = txt; }

const QString MessageCaller::getTitle() { return p_title; }

void MessageCaller::setBody(const QString &txt) { p_body = txt; }

const QString MessageCaller::getBody() { return p_body; }

MessageSelecter::MessageSelecter(QWidget *parent) : QFrame{parent} {
  setObjectName("message_selecter_bar");
  setContentsMargins(2, 0, 2, 0);
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 1, 2, 1);
  m_selecter = new QComboBox(this);
  m_selecter->addItem(tr("Select your Template to edit."));
  layout->addWidget(m_selecter, 0, 0, 1, 2);
  m_title = new QLabel(this);
  m_title->setToolTip(tr("Title"));
  layout->addWidget(m_title, 1, 0, 1, 1, Qt::AlignLeft);
  m_caller = new QLabel(this);
  m_caller->setToolTip(tr(""));
  layout->addWidget(m_caller, 1, 1, 1, 1, Qt::AlignRight);
  setLayout(layout);

  connect(m_selecter, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setContent(int)));
}

void MessageSelecter::setContent(int index) {
  MessageCaller cl = p_map.value(index);
  QString body = cl.getBody();
  if (body.isEmpty())
    return;

  QString ti = m_selecter->itemData(index, Qt::DisplayRole).toString();
  QString ca = m_selecter->itemData(index, Qt::UserRole).toString();
  if (ca.isEmpty())
    return;

  m_caller->setText(ca);
  m_title->setText(ti);
  emit sendBody(cl.getBody());
}

void MessageSelecter::setSelecters(const QList<MessageCaller> &list) {
  if (list.count() < 1)
    return;

  p_map.clear();
  m_selecter->clear();
  m_selecter->insertItem(0, tr("Select your Template to edit."));
  int index = 1;
  for (int i = 0; i < list.count(); i++) {
    MessageCaller cl = list.at(i);
    m_selecter->insertItem(index, cl.getTitle(), cl.getCaller());
    p_map.insert(index, cl);
    index++;
  }
}

const QList<MessageCaller> MessageSelecter::getSelecters() {
  QList<MessageCaller> list;
  for (int i = 1; i < m_selecter->count(); i++) {
    MessageCaller cl;
    cl.setCaller(m_selecter->itemData(i, Qt::UserRole).toString());
    cl.setTitle(m_selecter->itemData(i, Qt::DisplayRole).toString());
    list.append(cl);
  }
  return list;
}

const QString MessageSelecter::getCaller() {
  return m_caller->text().trimmed();
}

const QString MessageSelecter::getTitle() { return m_title->text().trimmed(); }
