// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGESEARCHBAR_H
#define ANTIQUACRM_UTILS_STORAGESEARCHBAR_H

#include <AntiquaWidgets>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class StorageSearchBar final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::ALineEdit *m_searchLine;
  QPushButton *btn_search;

private Q_SLOTS:
  void onSearchLineEdit(const QString &);
  void searchClicked();

Q_SIGNALS:
  void sendFindStorage(const QString &);

public:
  explicit StorageSearchBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_STORAGESEARCHBAR_H
