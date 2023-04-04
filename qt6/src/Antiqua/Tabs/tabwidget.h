// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_TABWIDGET_H
#define ANTIQUACRM_TABS_TABWIDGET_H

#include <AGlobal>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {

class TabsBar;

class ANTIQUACRM_LIBRARY TabWidget : public QTabWidget {
  Q_OBJECT

private:
  AntiquaCRM::TabsBar *m_tabBar;

public:
  explicit TabWidget(QWidget *parent = nullptr);
  static const QIcon defaultIcon();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_TABWIDGET_H
