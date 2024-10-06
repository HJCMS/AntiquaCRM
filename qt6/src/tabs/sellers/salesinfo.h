// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_SALESINFO_H
#define ANTIQUACRM_PLUGIN_SELLERS_SALESINFO_H

#include <AntiquaWidgets>
#include <QDateTime>

class SalesInfo final : public QWidget {
  Q_OBJECT

private:
  QLabel* m_orderPayed;
  AntiquaCRM::SelectPaymentMethod* m_method;
  AntiquaCRM::DateTimeInfo* m_since;
  AntiquaCRM::DateTimeInfo* m_confirmed;

  QLabel* infoCell(const QString&);

  const QDateTime convertDateTime(const QString&);

  void setPaymentMethod(int);

public Q_SLOTS:
  void setInfo(const QJsonObject& object);

public:
  explicit SalesInfo(QWidget* parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_SALESINFO_H
