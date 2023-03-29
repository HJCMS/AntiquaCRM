// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_STATUSBAR_H
#define ANTIQUA_UI_STATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QWidget>

class StatusBar final : public QStatusBar {
  Q_OBJECT

public:
  explicit StatusBar(QWidget *parent = nullptr);
};

#endif // ANTIQUA_UI_STATUSBAR_H
