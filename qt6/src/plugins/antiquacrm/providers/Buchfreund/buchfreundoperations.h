// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H
#define ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H

#include <AntiquaProviders>
#include <AntiquaWidgets>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QJsonObject>
#include <QLabel>
#include <QMap>
#include <QUrl>
#include <QWidget>

class ANTIQUACRM_LIBRARY BuchfreundOperations final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  AntiquaCRM::ANetworker *m_networker;
  QLabel *m_info;
  AntiquaCRM::TextLine *m_id;
  AntiquaCRM::SelectDeliverService *m_service;
  QComboBox *m_type;
  AntiquaCRM::TextLine *m_srv_number;
  QDateTimeEdit *m_dlv_date;
  QString p_order_id;
  QMap<QString, QVariant> p_config;
  QLabel *label(const QString &text);
  const QString deliveryDate();
  bool initConfiguration() override;
  const QUrl apiQuery(const QString &target) override;

private Q_SLOTS:
  void networkResponse(const QJsonDocument &document);
  void prepareOperation() override;

public:
  explicit BuchfreundOperations(QWidget *parent = nullptr);
  int exec(const QJsonObject &data) override;
};

#endif // ANTIQUACRM_PLUGIN_BUCHFREUND_OPERATIONS_H
