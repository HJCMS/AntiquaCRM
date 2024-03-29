// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSHEADERFRAME_H
#define ANTIQUACRM_CUSTOMERSHEADERFRAME_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QLabel>
#include <QWidget>

/**
 * @brief Customers header frame
 * @ingroup _customers
 */
class CustomersHeaderFrame : public QFrame {
  Q_OBJECT

private:
  QLabel *m_displayName;

public:
  SerialID *c_id;
  AntiquaDateInfo *c_since;
  AntiquaDateInfo *c_changed;

  /**
   * @brief CustomersHeaderFrame
   */
  explicit CustomersHeaderFrame(QWidget *parent = nullptr);
  void setDisplayName(const QString &);
};

#endif // ANTIQUACRM_CUSTOMERSHEADERFRAME_H
