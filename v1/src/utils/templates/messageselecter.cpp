// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messageselecter.h"
#include "messagecaller.h"

#include <QLayout>

MessageSelecter::MessageSelecter(QWidget *parent, bool edit) : QFrame{parent} {
  setObjectName("message_selecter_bar");
  setContentsMargins(2, 0, 2, 0);
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(2, 1, 2, 1);
  m_selecter = new AntiquaComboBox(this);
  m_selecter->setObjectName("tb_message");
  m_selecter->addItem(tr("Select your Template to edit."));
  layout->addWidget(m_selecter, 0, 0, 1, 1);
  m_genderBox = new GenderBox(this);
  m_genderBox->setObjectName("tb_gender");
  m_genderBox->setInfo(tr("Gender"));
  layout->addWidget(m_genderBox, 0, 1, 1, 1);
  m_title = new QLabel(this);
  m_title->setToolTip(tr("Title"));
  m_title->setVisible(edit);
  layout->addWidget(m_title, 1, 0, 1, 1, Qt::AlignLeft);
  m_caller = new QLabel(this);
  m_caller->setToolTip(tr("Constant"));
  m_caller->setEnabled(false);
  m_caller->setVisible(edit);
  layout->addWidget(m_caller, 1, 1, 1, 1, Qt::AlignRight);
  m_subject = new LineEdit(this);
  m_subject->setObjectName("tb_subject");
  m_subject->setInfo(tr("Subject"));
  layout->addWidget(m_subject, 2, 0, 1, 2);
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
  m_subject->setValue(obj.value("subject").toString());
  m_genderBox->setValue(obj.value("gender").toInt());
  setAttachment(obj.value("attachment").toBool());
  emit sendBody(obj.value("body").toString());
}

void MessageSelecter::setAttachment(bool b) {
  attachment = b;
  emit attachmentChanged(b);
}

void MessageSelecter::setGender(int gender) {
  for (int i = 0; i < m_selecter->count(); i++) {
    QJsonObject obj = m_selecter->itemData(i, Qt::UserRole).toJsonObject();
    if (obj.isEmpty())
      continue;

    if (obj.value("gender").toInt() == gender) {
      m_selecter->setCurrentIndex(i);
      break;
    }
  }
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

const QString MessageSelecter::getSubject() {
  return m_subject->value().toString().trimmed();
}

bool MessageSelecter::hasAttachment() { return attachment; }

int MessageSelecter::getGender() { return m_genderBox->value().toInt(); }
