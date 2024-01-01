// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H
#define ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H

#include <AGlobal>
#include <AntiquaProviders>
#include <QJsonObject>
#include <QMap>
#include <QUrl>
#include <QWidget>

class ANTIQUACRM_LIBRARY BuchfreundOperations final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  QString p_order_id;
  QMap<QString, QVariant> p_config;
  bool initConfiguration() override;
  const QUrl apiQuery(const QString &target) override;

private Q_SLOTS:
  void prepareOperation() override;

public:
  explicit BuchfreundOperations(QWidget *parent = nullptr);
  int exec(const QJsonObject &data) override;
};

#endif // ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H
