// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_LINEEDIT_H
#define ANTIQUA_WIDGETS_LINEEDIT_H

#include <AGlobal>
#include <AInputEdit>
#include <QLineEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @brief AntiquaCRM Line edit widget
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ALineEdit final : public QLineEdit, public AInputEdit {
  Q_OBJECT

private Q_SLOTS:
  void skipReturnPressed();

public Q_SLOTS:
  void setValue(const QVariant &) override;

public:
  explicit ALineEdit(QWidget *parent = nullptr);
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_LINEEDIT_H
