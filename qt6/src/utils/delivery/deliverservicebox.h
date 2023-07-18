// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DELIVERSERVICEBOX_H
#define ANTIQUACRM_UTILS_DELIVERSERVICEBOX_H

#include <QComboBox>
#include <QWheelEvent>
#include <QWidget>

class DeliverServiceBox : public QComboBox {
  Q_OBJECT

public Q_SLOTS:
  /**
   * @brief Setze Service mit "d_id"
   */
  void setCurrentServiceId(int did);

public:
  explicit DeliverServiceBox(QWidget *parent = nullptr);
  /**
   * @brief load Delivery Services from SQL Database
   */
  void initDeliverServices();

  /**
   * @brief Aktuelle Datenbank Service Nummer "d_id"
   */
  int getCurrentServiceId();
};

#endif // ANTIQUACRM_UTILS_DELIVERSERVICEBOX_H
