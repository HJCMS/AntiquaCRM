// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mailcommand_p.h"

#include <ASettings>
#include <QDebug>
#include <QDesktopServices>
#include <QProcess>
#include <QUrlQuery>

namespace AntiquaCRM
{

MailCommand::MailCommand(QObject* parent) : QProcess{parent} {
  p_email = QUrl();
  p_subject = QString();
  p_body = QString();
  p_attachment = QString();
}

bool MailCommand::initMailAppl() {
  QFileInfo _info;
  AntiquaCRM::ASettings cfg(this);
  _info.setFile(cfg.value("dirs/appl_mailler").toString());

  if (_info.isExecutable()) {
    qInfo("Using Mail-Application '%s'", qPrintable(_info.filePath()));
    setProgram(_info.filePath());
    return true;
  }

  qWarning("Missing Mail-Application!");
  emit sendMessage("Missing Mailler Application!");
  return false;
}

const QStringList MailCommand::prepare(MailCommand::Type t) {
  if (t == MailCommand::UNKNOWN) {
    // mailto {"to","cc","bcc","subject","body"}
    emit sendMessage(tr("Attachments are disabled in this mode!"));

    QUrlQuery query;
    query.addQueryItem("to", getMail());
    query.addQueryItem("subject", p_subject);
    query.addQueryItem("body", p_body.trimmed());

    QUrl url;
    url.setScheme("mailto");
    url.setQuery(query);
    return QStringList(url.toString(QUrl::FullyEncoded));
  }

  if (program().isEmpty()) {
    QString errStr = tr("Missing mail application");
    errStr.append(" - ");
    errStr.append(tr("please check your configuration!"));
    emit sendMessage(errStr);
    return QStringList();
  }

  QStringList cmd;
  // https://docs.microsoft.com/en-US/outlook/troubleshoot/security/information-about-email-security-settings
  if (t == MailCommand::OUTLOOK) {
    QStringList params;
    params << "subject=" + AntiquaCRM::AUtil::urlEncode(p_subject);
    params << "body=" + AntiquaCRM::AUtil::urlEncode(p_body);
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
  if (t == MailCommand::THUNDERBIRD) {
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

void MailCommand::sendMail() {
  initMailAppl();

  QStringList cmd;
  if (program().contains("thunderbird", Qt::CaseInsensitive)) {
    cmd = prepare(THUNDERBIRD);
  } else if (program().contains("outlook", Qt::CaseInsensitive)) {
    cmd = prepare(OUTLOOK);
  } else {
    QUrl url(prepare(UNKNOWN).join(""), QUrl::StrictMode);
    if (url.isValid())
      QDesktopServices::openUrl(url);

    cmd.clear(); // force empty
  }

  if (cmd.size() < 1) {
    terminate();
    return;
  }

  qint64 retval;
  setArguments(cmd);
  startDetached(&retval);
}

bool MailCommand::setMail(const QString& data) {
  if (!data.contains("@"))
    return false;

  QStringList l = data.split("@");
  QUrl url;
  url.setScheme("mailto");
  url.setUserInfo(l.first());
  url.setHost(l.last());
  if (url.isValid()) {
    p_email = url;
    return true;
  }
  emit sendMessage(tr("Invalid eMail Address"));
  return false;
}

const QString MailCommand::getMail() {
  return p_email.toString(QUrl::RemoveScheme).replace("/", "");
}

void MailCommand::setSubject(const QString& data) {
  if (data.isEmpty())
    return;

  p_subject = data;
}

void MailCommand::setBody(const QString& data) {
  if (data.isEmpty())
    return;

  p_body = data;
}

void MailCommand::setAttachment(const QString& data) {
  if (data.isEmpty())
    return;

  p_attachment = data;
}

} // namespace AntiquaCRM
