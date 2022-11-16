// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PURCHASETABLECOLUMN_H
#define ANTIQUACRM_PURCHASETABLECOLUMN_H

#include <QMetaType>
#include <QString>

class PurchaseTableColumn final {
private:
  QString p_f;
  QString p_n;

public:
  PurchaseTableColumn();
  explicit PurchaseTableColumn(const QString &f, const QString &n);

  /**
   * @brief SQL Tablefield name
   */
  const QString field();

  /**
   * @brief Header Display name
   */
  const QString name();

  /**
   * @brief Required Data Type
   */
  const QMetaType::Type type();
};

#endif // ANTIQUACRM_PURCHASETABLECOLUMN_H
