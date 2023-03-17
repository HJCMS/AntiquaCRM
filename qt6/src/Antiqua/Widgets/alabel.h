// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_WIDGETS_LABLE_H
#define ANTIQUA_WIDGETS_LABLE_H

#include <AGlobal>
#include <AInputEdit>
#include <QLabel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @ingroup AntiquaWidgets
 * @class ALabel
 * @brief AntiquaCRM Default Label widget
 */
class ANTIQUACRM_LIBRARY ALabel final : public QLabel, public AInputEdit {
  Q_OBJECT

public Q_SLOTS:
  void setValue(const QVariant &) override;
  void setRestrictions(const QSqlField &) override;

public:
  explicit ALabel(QWidget *parent = nullptr);
  explicit ALabel(const QString &text, QWidget *parent = nullptr);
  const QVariant getValue() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUA_WIDGETS_LABLE_H
