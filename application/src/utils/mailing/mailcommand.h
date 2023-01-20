// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILING_MAILCOMMAND_H
#define MAILING_MAILCOMMAND_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QUrl>
#include <QVariant>

class MailCommand final : public QProcess {
  Q_OBJECT

private:
  QUrl p_email;
  QString p_subject;
  QString p_body;
  QString p_attachment;
  QString p_signature;

  void setMailProgramm();

public Q_SLOTS:
  void sendMail();

Q_SIGNALS:
  void sendMessage(const QString &);

public:
  enum Type { UNKNOWN = 0, OUTLOOK = 1, THUNDERBIRD = 2 };
  Q_ENUM(Type);
  explicit MailCommand(QObject *parent = nullptr);
  const QString urlEncode(const QString &) const;
  void setMail(const QString &data);
  const QString getMail();
  void setSubject(const QString &data);
  void setBody(const QString &data);
  void setAttachment(const QString &data);

private:
  const QStringList prepare(MailCommand::Type t);
};

#endif // MAILING_MAILCOMMAND_H
