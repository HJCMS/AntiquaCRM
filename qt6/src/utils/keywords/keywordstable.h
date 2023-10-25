// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_KEYWORDS_TABLE_H
#define ANTIQUACRM_UTILS_KEYWORDS_TABLE_H

#include <AntiquaWidgets>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QSqlTableModel>
#include <QTableView>
#include <QVariant>

class KeywordsTable final : public QTableView {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql = nullptr;
  QSqlTableModel *m_tableModel = nullptr;
  AntiquaCRM::TableHeader *m_horizontalHeader;

private Q_SLOTS:
  void setItemRow(const QModelIndex &);

Q_SIGNALS:
  void signalRowSelected(const QSqlRecord &);

public:
  void select();

public:
  explicit KeywordsTable(QWidget *parent = nullptr);
  bool initTable(const QString &name = QString("categories_intern"));
  void commitQuery(const QSqlRecord &);
};

#endif // ANTIQUACRM_UTILS_KEYWORDS_TABLE_H
