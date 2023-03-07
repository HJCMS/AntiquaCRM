// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERS_CUSTOM_SEARCH_H
#define ANTIQUACRM_ORDERS_CUSTOM_SEARCH_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDate>
#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QWidget>

/**
 * @brief Orders Customer Search Dialog
 * @ingroup AntiquaOrders
 */
class OrdersCustomSearch final : public QDialog {
  Q_OBJECT

private:
  const QDate p_date;
  AntiquaComboBox *m_filterSelecter;
  AntiquaComboBox *m_yearSelect;
  QDialogButtonBox *m_btnBox;

  inline int currentFilter();
  inline int currentYear();

public:
  explicit OrdersCustomSearch(QWidget *parent = nullptr);
  const QString customQuery();
  int exec() override;
};

#endif // ANTIQUACRM_ORDERS_CUSTOM_SEARCH_H
