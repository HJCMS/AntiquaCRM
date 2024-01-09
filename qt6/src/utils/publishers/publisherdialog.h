// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_PUBLISHERDIALOG_H
#define ANTIQUACRM_UTILS_PUBLISHERDIALOG_H

#include <AntiquaWidgets>
#include <QPair>
#include <QObject>
#include <QWidget>

class PublisherTableView;
class PublisherEdit;

class PublisherDialog final : public AntiquaCRM::ADialog {
  Q_OBJECT

private:
  mutable QPair<QString, QString> p_tmp;
  PublisherTableView *m_table;
  PublisherEdit *m_edit;

private Q_SLOTS:
  void itemSelected(const QModelIndex &);
  void dataChanged();
  void saveData();

public:
  explicit PublisherDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_UTILS_DELIVERYDIALOG_H
