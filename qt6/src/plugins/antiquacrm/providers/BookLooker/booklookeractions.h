// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_ACTIONS_H
#define BOOKLOOKER_PLUGIN_ACTIONS_H

#include <AntiquaProviders>
#include <QLabel>
#include <QObject>
#include <QWidget>

class BookLookerOperations;

class ANTIQUACRM_LIBRARY BookLookerActions final
    : public AntiquaCRM::ProviderActionDialog {
  Q_OBJECT

private:
  QLabel *m_buyerInfo;
  BookLookerOperations *m_selecter;
  QString order_id;

Q_SIGNALS:
  void sendPostOperation(const QJsonObject &);

public Q_SLOTS:
  void prepareOperation() override;

public:
  explicit BookLookerActions(QWidget *parent);
  int exec(const QJsonObject &data) override;
};

#endif // BOOKLOOKER_PLUGIN_ACTIONS_H
