// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_COMBOBOX_H
#define ANTIQUA_WIDGETS_COMBOBOX_H

#include <AGlobal>
#include <AInputEdit>
#include <QComboBox>
#include <QObject>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class AComboBox
 * @brief AntiquaCRM Combobox widget
 */
class ANTIQUACRM_LIBRARY AComboBox final : public QComboBox, public AInputEdit {
  Q_OBJECT

private:
  /**
   * @brief mouse wheel support enabled or not
   */
  bool mouse_wheel_support = false;

protected:
  /**
   * @brief disable wheel events ...
   * to prevent unwanted changes on large input masks with scroll widget
   */
  void wheelEvent(QWheelEvent *e) override;

public Q_SLOTS:
  /**
   * @brief find and set item with Qt::UserRole value
   */
  void setValue(const QVariant &) override;

  /**
   * @brief get restriction from QSqlField
   */
  void setRestrictions(const QSqlField &) override;

public:
  explicit AComboBox(QWidget *parent = nullptr);

  /**
   * @brief set default text, no item selected.
   */
  const QString withoutDisclosures();

  /**
   * @brief create a Qt:UserRole value with withoutDisclosures() text.
   */
  void setWithoutDisclosures(const QVariant &data = QVariant());

  /**
   * @brief get current selected Qt::UserRole value
   */
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_COMBOBOX_H
