// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REFUNDING_PAGE_H
#define ANTIQUACRM_REFUNDING_PAGE_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY RefundingPage : public QWidget {
  Q_OBJECT

protected:
  AntiquaCRM::ASqlCore *m_sql = nullptr;

Q_SIGNALS:
  void sendForward();
  void sendBackward();

public:
  explicit RefundingPage(QWidget *parent = nullptr);
  virtual bool addDataRecord(qint64 id) = 0;
};

#endif // ANTIQUACRM_REFUNDINGS_PAGE_H
