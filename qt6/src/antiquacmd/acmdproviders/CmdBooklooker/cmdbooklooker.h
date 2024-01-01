// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_CMDBOOKLOOKER_H
#define ANTIQUACMD_CMDBOOKLOOKER_H

#include <QByteArray>
#include <QJsonDocument>
#include <QObject>

#ifndef ANTIQUACMD_PROVIDERS_H
#include "acmdproviders.h"
#endif

class CmdBooklooker final : public ACmdProviders {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACMD_INTERFACE FILE "cmdbooklooker.json")
  Q_INTERFACES(ACmdProviders)

private:
  const QString provider() const override;
  void initConfiguration() override;
  const QUrl apiQuery(const QString &action) override;
  void prepareContent(const QJsonDocument &document) override;
  const QString dateString(const QDate &date = QDate::currentDate()) const;
  void setTokenCookie(const QString &token);
  bool isCookieExpired();
  void authenticate();
  void query();

private Q_SLOTS:
  void responsed(const QByteArray &bread) override;

public Q_SLOTS:
  void start() override;

public:
  explicit CmdBooklooker(QObject *parent = nullptr);
  bool init() override;
};

#endif // ANTIQUACMD_CMDBOOKLOOKER_H
