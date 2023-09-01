// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_OPERATIONS_H
#define BOOKLOOKER_PLUGIN_OPERATIONS_H

#include <QObject>
#include <QWidget>
#include <QFrame>

class BookLookerOperations : public QFrame {
  Q_OBJECT

public:
  explicit BookLookerOperations(QWidget *parent = nullptr);
};

#endif // BOOKLOOKER_PLUGIN_OPERATIONS_H
