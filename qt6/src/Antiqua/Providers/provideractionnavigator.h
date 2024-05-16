// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PROVIDER_ACTION_NAVIGATOR_H
#define ANTIQUACRM_PROVIDER_ACTION_NAVIGATOR_H

#include <AGlobal>
#include <QFrame>
#include <QPushButton>
#include <QStackedWidget>

namespace AntiquaCRM {

/**
 * @class ProviderActionNavigator
 * @brief Navigator for Action pages
 *
 * @ingroup ProviderInterface
 */
class ANTIQUACRM_LIBRARY ProviderActionNavigator final : public QFrame {
  Q_OBJECT

private:
  QStackedWidget *stackedWidget;
  QPushButton *btn_back;
  QPushButton *btn_next;

private Q_SLOTS:
  void updateButtonAccess(int);
  void prepareGotoSignal();

Q_SIGNALS:
  void sendGotoPage(int);

public:
  /**
   * @param parent - parent object
   */
  explicit ProviderActionNavigator(QStackedWidget *pager,
                                   QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PROVIDER_ACTION_NAVIGATOR_H
