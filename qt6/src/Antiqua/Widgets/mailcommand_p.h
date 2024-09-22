// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_MAILCOMMAND_H
#define ANTIQUACRM_PRIVATE_MAILCOMMAND_H

#include <AntiquaCRM>
#include <QMetaObject>
#include <QObject>
#include <QProcess>
#include <QString>
#include <QUrl>
#include <QVariant>

namespace AntiquaCRM
{

class ANTIQUACRM_LIBRARY MailCommand final : public QProcess {
  Q_OBJECT

public:
  enum Type {
    UNKNOWN = 0,    /**< No Attachments action */
    OUTLOOK = 1,    /**< Outlook path is set */
    THUNDERBIRD = 2 /**< Thunderbird path is set */
  };
  Q_ENUM(Type);

private:
  QUrl p_email;
  QString p_subject;
  QString p_body;
  QString p_attachment;
  QString p_signature;
  bool initMailAppl();
  const QStringList prepare(MailCommand::Type t);

Q_SIGNALS:
  void sendMessage(const QString& message);

public Q_SLOTS:
  void sendMail();

public:
  explicit MailCommand(QObject* parent = nullptr);
  bool setMail(const QString& data);
  const QString getMail();
  void setSubject(const QString& data);
  void setBody(const QString& data);
  void setAttachment(const QString& data);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_MAILCOMMAND_H
