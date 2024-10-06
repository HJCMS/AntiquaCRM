// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_CREDITWORTHINESS_H
#define ANTIQUACRM_PLUGIN_SELLERS_CREDITWORTHINESS_H

#include <AntiquaWidgets>
#include <QLabel>

class Creditworthiness final : public QLabel {
  Q_OBJECT

public:
  explicit Creditworthiness(QWidget* parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_CREDITWORTHINESS_H
