// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_LABLE_H
#define ANTIQUACRM_INPUT_LABLE_H

#include <AGlobal>
#include <QLabel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ALabel
 * @brief Default Display Text Widget for AntiquaCRM.
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ALabel final : public QLabel {
  Q_OBJECT

public:
  /**
   * @brief Compact use for Qt::Alignment flags.
   * @code
   *  Example:
   *   ALabel::MiddleRight = (Qt::AlignVCenter | Qt::AlignRight)
   * @endcode
   */
  enum Align {
    TopLeft = 1,     /**< @brief align top left */
    MiddleLeft = 2,  /**< @brief align middle left */
    TopRight = 3,    /**< @brief align top left */
    MiddleRight = 4, /**< @brief align middle right */
    MiddleCenter = 5 /**< @brief align middle center */
  };

  /**
   * @param parent - parent object
   */
  explicit ALabel(QWidget *parent = nullptr);

  /**
   * @param text = Standard QLabel setText is used
   * @param parent - parent object
   */
  explicit ALabel(const QString &text, QWidget *parent = nullptr);

  /**
   * @param text = display text
   * @param align = Alignment before input text
   * @param parent - parent object
   */
  explicit ALabel(const QString &text,
                  ALabel::Align align = ALabel::MiddleLeft,
                  QWidget *parent = nullptr);

public Q_SLOTS:
  /**
   * @brief Change Alignment and set Text.
   * @param align - Text alignment
   * @param text - Label text
   */
  void setAlignText(ALabel::Align align, const QString &text);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_LABLE_H
