// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGESELECTER_UTILS_H
#define MESSAGESELECTER_UTILS_H

#include <QComboBox>
#include <QFrame>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

#include <Utils>

class MessageCaller final {
private:
  QString p_caller;
  QString p_title;
  QString p_body;
  int p_gender;

public:
  explicit MessageCaller();
  explicit MessageCaller(const QJsonObject &);
  void setCaller(const QString &);
  const QString getCaller();
  void setTitle(const QString &);
  const QString getTitle();
  void setBody(const QString &);
  const QString getBody();
  void setGender(int gender);
  int getGender();
  const QJsonObject toJson();
};

class MessageSelecter final : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_selecter;
  GenderBox *m_genderBox;
  QLabel *m_title;
  QLabel *m_caller;
  QMap<int, MessageCaller> p_map;

private Q_SLOTS:
  void setContent(int);

Q_SIGNALS:
  void sendBody(const QString &);
  void sendGender(int);

public:
  explicit MessageSelecter(QWidget *parent = nullptr);
  void setSelecters(const QList<MessageCaller> &list);
  const QList<MessageCaller> getSelecters();
  const QString getCaller();
  const QString getTitle();
  int getGender();
};

#endif // MESSAGESELECTER_UTILS_H
