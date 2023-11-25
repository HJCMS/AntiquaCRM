// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_PAGE_H
#define ANTIQUACRM_REFUNDING_PAGE_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY RefundingPage : public QWidget {
  Q_OBJECT

protected:
  AntiquaCRM::ASqlCore *m_sql = nullptr;

  /**
   * @brief order id
   * @see initPageData
   */
  qint64 p_order_id = -1;

  /**
   * @brief refunding cost
   * @see setRefundingConfig
   */
  QJsonObject p_refunds_config;

  double refundsCost();

Q_SIGNALS:
  /**
   * @brief send signal go forward
   */
  void sendForward();

  /**
   * @brief send signal go backward
   */
  void sendBackward();

  /**
   * @brief Notify progress status of this page
   */
  void sendStatus(bool);

public:
  /**
   * @param parent - pointer to parent object
   */
  explicit RefundingPage(QWidget *parent = nullptr);

  /**
   * @brief setting refunding configuration
   * @param conf - database settings
   */
  void setRefundingConfig(const QJsonObject &conf);

  /**
   * @brief SQL query with id, normaly order Id.
   * @param con - SQL connection
   * @param id  - order id
   */
  virtual bool initPageData(AntiquaCRM::ASqlCore *con, qint64 id) = 0;
};

#endif // ANTIQUACRM_REFUNDINGS_PAGE_H
