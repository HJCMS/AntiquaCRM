// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_BUYERINFO_H
#define ANTIQUACRM_PLUGIN_SELLERS_BUYERINFO_H

#include <AGlobal>
#include <QTextEdit>
#include <QWidget>

class ANTIQUACRM_LIBRARY BuyerInfo final : public QWidget {
  Q_OBJECT

private:
  QTextEdit *m_address;
  QTextEdit *m_comment;

public Q_SLOTS:
  void setAddress(const QString &str);
  void setComment(const QString &str);

public:
  explicit BuyerInfo(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_BUYERINFO_H
