// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DESIGNATIONS_LIST_H
#define DESIGNATIONS_LIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QSqlQuery>
#include <QWidget>

#include "designationdata.h"

class DesignationsList;

/**
 * @brief The DesignationsListItem class
 * @ingroup _designations
 */
class DesignationsListItem : public QListWidgetItem {

private:
  DesignationData data_t;

public:
  explicit DesignationsListItem(const DesignationData &data,
                                DesignationsList *parent = nullptr);
  const DesignationData getData();
};

/**
 * @brief The DesignationsList class
 * @ingroup _designations
 */
class DesignationsList : public QListWidget {
  Q_OBJECT

private Q_SLOTS:
  void setPrepareClicked(QListWidgetItem *);

Q_SIGNALS:
  void sendEditItem(const DesignationData &);

public:
  explicit DesignationsList(QWidget *parent = nullptr);
  void insertQuery(const QSqlQuery &);
};

#endif // DESIGNATIONS_LIST_H
