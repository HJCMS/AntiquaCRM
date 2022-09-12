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
  QString p_subject;
  QString p_body;
  int p_gender;
  bool p_attachment;

public:
  explicit MessageCaller();
  explicit MessageCaller(const QJsonObject &);
  void setCaller(const QString &);
  const QString getCaller();
  void setTitle(const QString &);
  const QString getTitle();
  void setSubject(const QString &);
  const QString getSubject();
  void setBody(const QString &);
  const QString getBody();
  void setGender(int gender);
  int getGender();
  void setAttachment(bool b = false);
  int getAttachment();
  const QJsonObject toJson();
};

class MessageSelecter final : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool attachment READ hasAttachment WRITE setAttachment NOTIFY
                 attachmentChanged)

private:
  bool attachment;
  QComboBox *m_selecter;
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

#endif // MESSAGESELECTER_UTILS_H
