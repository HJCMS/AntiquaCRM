// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_MESSAGESELECTER_H
#define TEMPLATES_MESSAGESELECTER_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QMap>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

#include "messagecaller.h"

class MessageSelecter final : public QFrame {
  Q_OBJECT
  Q_PROPERTY(bool attachment READ hasAttachment WRITE setAttachment NOTIFY
                 attachmentChanged)

private:
  bool attachment;
  AntiquaComboBox *m_selecter;
  GenderBox *m_genderBox;
  QLabel *m_title;
  QLabel *m_caller;
  LineEdit *m_subject;
  QMap<int, MessageCaller> p_map;

private Q_SLOTS:
  void setContent(int);

Q_SIGNALS:
  void attachmentChanged(bool);
  void sendBody(const QString &);
  void sendGender(int);

public Q_SLOTS:
  void setAttachment(bool);
  void setGender(int);

public:
  explicit MessageSelecter(QWidget *parent = nullptr, bool edit = false);
  void setSelecters(const QList<MessageCaller> &list);
  const QList<MessageCaller> getSelecters();
  const QString getCaller();
  const QString getTitle();
  const QString getSubject();
  bool hasAttachment();
  int getGender();
};

#endif // TEMPLATES_MESSAGESELECTER_H
