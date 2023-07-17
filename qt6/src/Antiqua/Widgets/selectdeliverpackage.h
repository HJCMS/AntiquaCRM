// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_SELECTDELIVERPACKAGE_H
#define ANTIQUACRM_WIDGETS_SELECTDELIVERPACKAGE_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY SelectDeliverPackage
    : public AntiquaCRM::AInputWidget {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  AComboBox *m_edit;
  ALabel *m_price;
  void initData() override{/* unused */};

private Q_SLOTS:
  void valueChanged(int);

public Q_SLOTS:
  void loadPackages(int);

  void setValue(const QVariant &) override;

  void setFocus() override;

  void reset() override;

public:
  /**
   * @param parent - parent widget
   */
  explicit SelectDeliverPackage(QWidget *parent = nullptr);
  ~SelectDeliverPackage();

  void setRestrictions(const QSqlField &) override;

  void setInputToolTip(const QString &) override;

  void setBuddyLabel(const QString &) override;

  bool isValid() override;

  const QMetaType getType() const override;

  const QVariant getValue() override;

  const QString popUpHints() override;

  const QString statusHints() override;

  /**
   * @brief Paket Preis "d_price"
   */
  qreal getPackagePrice();

  /**
   * @brief International oder National "d_international"
   */
  bool isInternational();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_SELECTDELIVERPACKAGE_H
