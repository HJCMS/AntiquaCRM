// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_MAILTEMPLATEKEYS_H
#define ANTIQUACRM_PRIVATE_MAILTEMPLATEKEYS_H

#include <AntiquaCRM>
#include <QMap>
#include <QSqlQuery>
#include <QThread>
#include <QVariant>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY MailTemplateKeys final : public QThread {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QMap<QString, QVariant> p_data;
  const QString stringValue(const QString &key);
  const QString zerofilled(const QString &key);
  bool checkRequirements();
  const QString salutation();
  const QString completeName();
  const QString customerMail();
  const QString customerId();
  const QString articleList();

Q_SIGNALS:
  void sendSuccess();
  void sendNoResult();

public:
  explicit MailTemplateKeys(QObject *parent = nullptr);
  bool appendData(QMap<QString, QVariant> &);
  const QVariant getData(const QString &key);
  const QString convert(const QString &key);
  void run() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_MAILTEMPLATEKEYS_H
