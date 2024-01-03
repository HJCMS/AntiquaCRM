// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_CMDABEBOOKS_H
#define ANTIQUACMD_CMDABEBOOKS_H

#include <QByteArray>
#include <QJsonDocument>
#include <QRegularExpression>
#include <QObject>

#ifndef ANTIQUACMD_PROVIDERS_H
#include "acmdproviders.h"
#endif

class AbeBooksDocument;

class CmdAbeBooks final : public ACmdProviders {
  Q_OBJECT
  Q_PLUGIN_METADATA(IID ANTIQUACMD_INTERFACE FILE "cmdabebooks.json")
  Q_INTERFACES(ACmdProviders)

private:
  AbeBooksDocument initDocument();
  inline const QRegularExpression charsetPattern() const;
  const QString provider() const override;
  void initConfiguration() override;
  const QUrl apiQuery(const QString &action) override;
  void prepareContent(const QJsonDocument &document) override;
  void prepareContent(const QDomDocument &document);

private Q_SLOTS:
  void responsed(const QByteArray &bread) override;

public Q_SLOTS:
  void start() override;

public:
  explicit CmdAbeBooks(QObject *parent = nullptr);
  bool init() override;
};

#endif // ANTIQUACMD_CMDABEBOOKS_H
