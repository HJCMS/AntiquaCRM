// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_SEARCHBARFILTER_H
#define ANTIQUACRM_TABS_SEARCHBARFILTER_H

#include <AGlobal>
#include <QComboBox>
#include <QIcon>
#include <QString>
#include <QWheelEvent>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class TabsSearchBarFilter
 * @brief Abstract filter combobox for Tabs searchbars
 * @ingroup TabsInterface
 */
class ANTIQUACRM_LIBRARY TabsSearchBarFilter : public QComboBox {
  Q_OBJECT

private:
  /**
   * @brief mouse wheel support enabled or not
   */
  bool wheel_support = false;

  /**
   * @brief disable wheel events ...
   * to prevent unwanted changes on large input masks with scroll widget
   */
  void wheelEvent(QWheelEvent *e) override;

protected:
  /**
   * @brief Search filter Item icon
   */
  static const QIcon defaultIcon();

  /**
   * @brief reserved for combo box items creation
   */
  virtual void createItemData() = 0;

public:
  /**
   * @param parent - parent object
   * @param mouseEvents - enable mouse wheel events
   */
  explicit TabsSearchBarFilter(QWidget *parent = nullptr,
                               bool mouseEvents = true);

  /**
   * @brief get selected filter
   * @param index - selected index
   * @return SQL-Columns separated by comma
   */
  const QString currentFilter(int index = -1);

  /**
   * @brief get selected filter
   * @param index - selected index
   */
  AntiquaCRM::SearchBarFilter getValue(int index = -1);

  /**
   * @brief get selected filter tooltip
   * @param index - selected index
   */
  const QString currentToolTip(int index = -1);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_SEARCHBARFILTER_H
