// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_BOOKLOOKER_H
#define ANTIQUACMD_BOOKLOOKER_H

#include <QByteArray>
#include <QJsonDocument>
#include <QObject>

#ifndef ANTIQUACMD_PROVIDERS_H
#include "acmdproviders.h"
#endif

class Booklooker final : public ACmdProviders {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACMD_INTERFACE FILE "booklooker.json")
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
  explicit Booklooker(QObject *parent = nullptr);
  bool init() override;
};

#endif // ANTIQUACMD_BOOKLOOKER_H
