// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_VIEWS_INDEX_H
#define ANTIQUACRM_VIEWS_INDEX_H

#include "viewsconfig.h"
#include <AntiquaTabs>
#include <QLabel>
#include <QObject>
#include <QStatusBar>
#include <QTableView>
#include <QWidget>

class ViewsTableModel;

class ANTIQUACRM_VIEWS_PLUGIN ViewsIndex final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  AntiquaCRM::AComboBox* m_comboBox;
  QTableView* m_tableView;
  ViewsTableModel* m_tableModel;
  QStatusBar* m_statusBar;
  void setDefaultTableView() override;

public Q_SLOTS:
  void createSearchQuery(const QString& title) override;

public:
  explicit ViewsIndex(QWidget* parent = nullptr);
  void openStartPage() override;
  void createNewEntry() override {};
  void openEntry(qint64) override{};
  void onEnterChanged() override;
  const QString getTitle() const override;
  bool customAction(const QJsonObject& obj) override;
  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_VIEWS_INDEX_H
