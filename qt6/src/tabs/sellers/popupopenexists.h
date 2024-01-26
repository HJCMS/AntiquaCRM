// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_POPUPOPENEXISTS_H
#define ANTIQUACRM_PLUGIN_SELLERS_POPUPOPENEXISTS_H

#include <QMessageBox>
#include <QObject>
#include <QWidget>

class PopUpOpenExists final : public QMessageBox {
  Q_OBJECT

public:
  explicit PopUpOpenExists(QWidget *parent = nullptr);
  void setMessage(const QStringList &data);
  int exec() override;
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_POPUPOPENEXISTS_H
