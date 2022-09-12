// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailcommand.h"
#include "applsettings.h"

#include <QDebug>
#include <QProcess>

#ifndef MAIL_CFG_PROGRAMM
#define MAIL_CFG_PROGRAMM QString("dirs/mailapplication")
#endif

MailCommand::MailCommand(QObject *parent) : QProcess{parent} {
  p_email = QUrl();
  p_subject = QString();
  p_body = QString();
  p_attachment = QString();
  ApplSettings cfg(this);
  p_mailler = cfg.value(MAIL_CFG_PROGRAMM).toString();
  setProgram(p_mailler);
}

const QString MailCommand::urlEncode(const QString &str) const {
  QByteArray data = QUrl::toPercentEncoding(str);
  return QString::fromLocal8Bit(data);
}

void MailCommand::sendMail() {
  if (program().isEmpty()) {
    qWarning("Missing Mailler Application");
    return;
  }

  QStringList cmd;
  if (p_mailler.contains("thunderbird", Qt::CaseInsensitive)) {
    cmd = prepare(THUNDERBIRD);
  } else if (p_mailler.contains("outlook", Qt::CaseInsensitive)) {
    cmd = prepare(OUTLOOK);
  }

  if (cmd.size() < 1) {
    terminate();
    return;
  }

  qint64 retval;
  setArguments(cmd);
  startDetached(&retval);
}

void MailCommand::setMail(const QString &data) {
  if (!data.contains("@"))
    return;

  QStringList l = data.split("@");
  QUrl url;
  url.setScheme("mailto");
  url.setUserInfo(l.first());
  url.setHost(l.last());
  if (url.isValid())
    p_email = url;
}

const QString MailCommand::getMail() {
  return p_email.toString(QUrl::RemoveScheme).replace("/", "");
}

void MailCommand::setSubject(const QString &data) {
  if (data.isEmpty())
    return;

  p_subject = data;
}

void MailCommand::setBody(const QString &data) {
  if (data.isEmpty())
    return;

  p_body = data;
}

void MailCommand::setAttachment(const QString &data) {
  if (data.isEmpty())
    return;

  p_attachment = data;
}

const QStringList MailCommand::prepare(MailCommand::Type t) {
  QStringList cmd;
  // https://docs.microsoft.com/en-US/outlook/troubleshoot/security/information-about-email-security-settings
  if (t == OUTLOOK) {
    QStringList params;
    params << "subject=" + urlEncode(p_subject);
    params << "body=" + urlEncode(p_body);
    cmd.clear();
    cmd << "-c";
    cmd << "IPM.Note";
    cmd << "/m";
    cmd << getMail() + "?" + params.join("&");
    if (!p_attachment.isEmpty()) {
      cmd << "/a" << p_attachment;
    }
    return cmd;
  }

  // https://kb.mozillazine.org/Command_line_arguments_%28Thunderbird%29
  if (t == THUNDERBIRD) {
    cmd << "-compose";
    QStringList params;
    params << "format=1";
    params << "to='" + getMail() + "'";
    params << "subject='" + p_subject + "'";
    params << "body='" + p_body.trimmed() + "'";
    if (!p_attachment.isEmpty()) {
      params << "attachment=" + p_attachment;
    }
    cmd << params.join(",");
    return cmd;
  }

  return QStringList();
}