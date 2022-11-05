// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_MESSAGECALLER_H
#define TEMPLATES_MESSAGECALLER_H

#include <QString>
#include <QJsonObject>

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
  MessageCaller(const MessageCaller &other);
  MessageCaller(const QJsonObject &);
  MessageCaller &operator=(const MessageCaller &other);
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

#endif // TEMPLATES_MESSAGECALLER_H
