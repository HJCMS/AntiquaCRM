// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_INDEX_H
#define ANTIQUACRM_REPORTS_INDEX_H

#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class ReportsToolBar;

class ANTIQUACRM_LIBRARY ReportsIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  ReportsToolBar *m_toolBar;
  void setDefaultTableView() override;

private Q_SLOTS:
  void selectMonth(const QDate &);

public:
  explicit ReportsIndex(QWidget *parent = nullptr);
  void openStartPage() override{};
  void createSearchQuery(const QString &) override{};
  void createNewEntry() override{};
  void openEntry(qint64) override{};
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject &obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_REPORTS_INDEX_H
