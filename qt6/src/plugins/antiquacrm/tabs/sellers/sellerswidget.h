// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_WIDGET_H
#define ANTIQUACRM_PLUGIN_SELLERS_WIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class SellersSalesTab;
class SellersSalesList;

class ANTIQUACRM_LIBRARY SellersWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  SellersSalesTab *m_pages;
  SellersSalesList *m_tree;

  void popupWarningTabInEditMode() override{/* unused */};
  void setDefaultTableView() override{/* unused */};

  bool findPage(const QString &provider, const QString &oid);

private Q_SLOTS:
  void openProviderDialog();
  void openOrderPage(const QString &provider, const QString &oid);

public:
  explicit SellersWidget(QWidget *parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString &history = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 prid) override;

  void onEnterChanged() override;

  const QString getTitle() const override;

  bool customAction(const QJsonObject &obj) override;

  const QStringList acceptsCustomActions() const override;
};

#endif
