// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_DATEINFO_H
#define ANTIQUA_WIDGETS_DATEINFO_H

#include <AGlobal>
#include <AInputEdit>
#include <QDate>
#include <QDateTimeEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @def ANTIQUA_WIDGET_FALLBACK_DATE
 * @brief Antiqua Widget fallback date if no valid data given
 */
#ifndef ANTIQUA_WIDGET_FALLBACK_DATE
#define ANTIQUA_WIDGET_FALLBACK_DATE QDate(1970, 01, 01)
#endif

/**
 * @ingroup AntiquaWidgets
 * @class ADateInfo
 * @brief AntiquaCRM Date info widget
 */
class ANTIQUACRM_LIBRARY ADateInfo final : public QDateTimeEdit,
                                           public AInputEdit {
  Q_OBJECT

public Q_SLOTS:
  /**
   * @brief set date time with datetime string or object
   */
  void setValue(const QVariant &) override;

  /**
   * @brief find input restriction with QSqlField info
   */
  void setRestrictions(const QSqlField &) override;

public:
  explicit ADateInfo(QWidget *parent = nullptr);

  /**
   * @brief current date time object
   */
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_DATEINFO_H
