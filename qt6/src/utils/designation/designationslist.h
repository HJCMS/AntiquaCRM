// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DESIGNATIONSLIST_H
#define ANTIQUACRM_UTILS_DESIGNATIONSLIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QSqlQuery>
#include <QWidget>

#include "designationdata.h"

class DesignationsList;

class DesignationsListItem : public QListWidgetItem {

private:
  DesignationData data_t;

public:
  explicit DesignationsListItem(const DesignationData &data,
                                DesignationsList *parent = nullptr);
  const DesignationData getData();
};

class DesignationsList : public QListWidget {
  Q_OBJECT

private Q_SLOTS:
  void setPrepareClicked(QListWidgetItem *);

Q_SIGNALS:
  void sendEditItem(const DesignationData &);

public:
  explicit DesignationsList(QWidget *parent = nullptr);
  void insertQuery(QSqlQuery &);
};

#endif // ANTIQUACRM_UTILS_DESIGNATIONSLIST_H
