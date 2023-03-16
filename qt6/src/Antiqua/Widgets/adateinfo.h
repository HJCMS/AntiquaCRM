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
#include <QDateTimeEdit>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @brief AntiquaCRM Date info widget
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ADateInfo final : public QDateTimeEdit,
                                           public AInputEdit {
  Q_OBJECT

public Q_SLOTS:
  void setValue(const QVariant &) override;

public:
  explicit ADateInfo(QWidget *parent = nullptr);
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_DATEINFO_H
