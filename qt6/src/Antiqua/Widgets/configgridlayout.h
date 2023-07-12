// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_DIALOGS_CONFIGGRIDLAYOUT_H
#define ANTIQUACRM_DIALOGS_CONFIGGRIDLAYOUT_H

#include <QGridLayout>

namespace AntiquaCRM {

class ConfigGridLayout final : public QGridLayout {
  Q_OBJECT

public:
  explicit ConfigGridLayout(QWidget *parent);
  void addToolTip(int row, int column, const QString &text);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_DIALOGS_CONFIGGRIDLAYOUT_H
