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
  p_gender = 0;
}

MessageCaller::MessageCaller(const QJsonObject &jso) {
  p_caller = jso.value("caller").toString();
  p_title = jso.value("title").toString();
  p_body = jso.value("body").toString();
  p_gender = jso.value("gender").toInt();
}

void MessageCaller::setCaller(const QString &txt) { p_caller = txt; }

const QString MessageCaller::getCaller() { return p_caller; }

void MessageCaller::setTitle(const QString &txt) { p_title = txt; }

const QString MessageCaller::getTitle() { return p_title; }

void MessageCaller::setBody(const QString &txt) { p_body = txt; }

const QString MessageCaller::getBody() { return p_body; }

void MessageCaller::setGender(int gender) { p_gender = gender; }

int MessageCaller::getGender() { return p_gender; }

const QJsonObject MessageCaller::toJson() {
  QJsonObject jso;
  jso.insert("caller", QJsonValue(p_caller));
  jso.insert("title", QJsonValue(p_title));
  jso.insert("body", QJsonValue(p_body));
  jso.insert("gender", QJsonValue(p_gender));
  return jso;
}

MessageSelecter::MessageSelecter(QWidget *parent) : QFrame{parent} {
  setObjectName("message_selecter_bar");
  setContentsMargins(2, 0, 2, 0);
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 1, 2, 1);
  m_selecter = new QComboBox(this);
  m_selecter->setObjectName("tb_message");
  m_selecter->addItem(tr("Select your Template to edit."));
  layout->addWidget(m_selecter, 0, 0, 1, 1);
  m_genderBox = new GenderBox(this);
  m_genderBox->setObjectName("tb_gender");
  m_genderBox->setInfo(tr("Gender"));
  layout->addWidget(m_genderBox, 0, 1, 1, 1);
  m_title = new QLabel(this);
  m_title->setToolTip(tr("Title"));
  layout->addWidget(m_title, 1, 0, 1, 1, Qt::AlignLeft);
  m_caller = new QLabel(this);
  m_caller->setToolTip(tr("Constant"));
  layout->addWidget(m_caller, 1, 1, 1, 1, Qt::AlignRight);
  setLayout(layout);

  connect(m_selecter, SIGNAL(currentIndexChanged(int)), this,
          SLOT(setContent(int)));
}

void MessageSelecter::setContent(int index) {
  QJsonObject obj = m_selecter->itemData(index, Qt::UserRole).toJsonObject();
  if (obj.isEmpty())
    return;

  m_caller->setText(obj.value("caller").toString());
  m_title->setText(obj.value("title").toString());
  qDebug() << obj.value("gender").toInt();
  m_genderBox->setValue(obj.value("gender").toInt());
  emit sendBody(obj.value("body").toString());
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
    m_selecter->insertItem(index, cl.getTitle(), cl.toJson());
    p_map.insert(index, cl);
    index++;
  }
}

const QList<MessageCaller> MessageSelecter::getSelecters() {
  QList<MessageCaller> list;
  for (int i = 1; i < m_selecter->count(); i++) {
    QJsonObject obj = m_selecter->itemData(i, Qt::UserRole).toJsonObject();
    if (obj.isEmpty())
      continue;

    MessageCaller cl(obj);
    list.append(cl);
  }
  return list;
}

const QString MessageSelecter::getCaller() {
  return m_caller->text().trimmed();
}

const QString MessageSelecter::getTitle() { return m_title->text().trimmed(); }

int MessageSelecter::getGender() { return m_genderBox->value().toInt(); }
