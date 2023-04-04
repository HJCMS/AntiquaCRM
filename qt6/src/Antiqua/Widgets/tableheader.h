// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_TABLEHEADER_H
#define ANTIQUACRM_WIDGETS_TABLEHEADER_H

#include <AGlobal>
#include <QHeaderView>

namespace AntiquaCRM {

/**
 * @class TableHeader
 * @brief Horizontal Table Header for TableViews
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY TableHeader final : public QHeaderView {
  Q_OBJECT

public Q_SLOTS:
  void resizeToContents(bool);

public:
  explicit TableHeader(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_TABLEHEADER_H
