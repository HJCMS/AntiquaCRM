// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILCOMMAND_UTILS_H
#define MAILCOMMAND_UTILS_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QUrl>
#include <QVariant>

class MailCommand final : public QProcess {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public Q_SLOTS:
  void sendMail();

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
  QString p_mailler;
  QUrl p_email;
  QString p_subject;
  QString p_body;
  QString p_attachment;
  const QStringList prepare(MailCommand::Type t);
};

#endif // MAILCOMMAND_UTILS_H
