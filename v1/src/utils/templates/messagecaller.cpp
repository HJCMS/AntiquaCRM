// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagecaller.h"

MessageCaller::MessageCaller() {
  p_caller = QString();
  p_title = QString();
  p_subject = QString();
  p_body = QString();
  p_gender = 0;
  p_attachment = false;
}

MessageCaller::MessageCaller(const MessageCaller &other) {
  p_caller = other.p_caller;
  p_title = other.p_title;
  p_subject = other.p_subject;
  p_body = other.p_body;
  p_gender = other.p_gender;
  p_attachment = other.p_attachment;
}

MessageCaller::MessageCaller(const QJsonObject &jso) {
  p_caller = jso.value("caller").toString();
  p_title = jso.value("title").toString();
  p_subject = jso.value("subject").toString();
  p_body = jso.value("body").toString();
  p_gender = jso.value("gender").toInt();
  p_attachment = jso.value("attachment").toBool();
}

MessageCaller &MessageCaller::operator=(const MessageCaller &other) {
  if (this == &other)
    return *this;

  p_caller = other.p_caller;
  p_title = other.p_title;
  p_subject = other.p_subject;
  p_body = other.p_body;
  p_gender = other.p_gender;
  p_attachment = other.p_attachment;
  return *this;
}

void MessageCaller::setCaller(const QString &txt) { p_caller = txt; }

const QString MessageCaller::getCaller() { return p_caller; }

void MessageCaller::setTitle(const QString &txt) { p_title = txt; }

const QString MessageCaller::getTitle() { return p_title; }

void MessageCaller::setSubject(const QString &txt) { p_subject = txt; }

const QString MessageCaller::getSubject() { return p_subject; }

void MessageCaller::setBody(const QString &txt) { p_body = txt; }

const QString MessageCaller::getBody() { return p_body; }

void MessageCaller::setGender(int gender) { p_gender = gender; }

int MessageCaller::getGender() { return p_gender; }

void MessageCaller::setAttachment(bool b) { p_attachment = b; }

int MessageCaller::getAttachment() { return p_attachment; }

const QJsonObject MessageCaller::toJson() {
  QJsonObject jso;
  jso.insert("caller", QJsonValue(p_caller));
  jso.insert("title", QJsonValue(p_title));
  jso.insert("subject", QJsonValue(p_subject));
  jso.insert("body", QJsonValue(p_body));
  jso.insert("gender", QJsonValue(p_gender));
  jso.insert("attachment", QJsonValue(p_attachment));
  return jso;
}
