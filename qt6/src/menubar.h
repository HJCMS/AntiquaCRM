// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_MENUBAR_H
#define ANTIQUA_UI_MENUBAR_H

#include <QMenuBar>
#include <QObject>
#include <QWidget>

class MenuBar final : public QMenuBar {
  Q_OBJECT

public:
  explicit MenuBar(QWidget *parent = nullptr);
};

#endif // ANTIQUA_UI_MENUBAR_H
