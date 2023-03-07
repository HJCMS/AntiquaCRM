// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSEARCHLINE_H
#define ANTIQUACRM_ORDERSEARCHLINE_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Orders Customer Search Line Edit
 * @ingroup widgets
 */
class OrderSearchLine final : public SearchLineEdit {
  Q_OBJECT

public:
  OrderSearchLine(QWidget *parent = nullptr);
  const QString getSearch() override;
};

#endif // ANTIQUACRM_ORDERSEARCHLINE_H
