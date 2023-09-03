// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_SELLERSTATUS_H
#define ABEBOOKS_PLUGIN_SELLERSTATUS_H

#include <AGlobal>
#include <QWidget>

class ANTIQUACRM_LIBRARY AbeBooksSellerStatus final : public QWidget {
  Q_OBJECT

Q_SIGNALS:
  void sendSelectionModified(bool);

public:
  explicit AbeBooksSellerStatus(QWidget *parent = nullptr);
};

#endif // ABEBOOKS_PLUGIN_SELLERSTATUS_H
