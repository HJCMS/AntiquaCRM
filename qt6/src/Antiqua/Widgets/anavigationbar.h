// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_NAVIGATIONBAR_H
#define ANTIQUACRM_INPUT_NAVIGATIONBAR_H

#include <AntiquaCRM>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ANavigationBar
 * @brief Antiqua Navigation bar
 *
 * @ingroup EditInputs
 */
class ANTIQUACRM_LIBRARY ANavigationBar : public QWidget {
  Q_OBJECT

private:
  int p_count;
  QPushButton *btn_back;
  QPushButton *btn_next;

Q_SIGNALS:
  void sendPrev();
  void sendNext();

public Q_SLOTS:
  void setIndex(int index = -1);

public:
  explicit ANavigationBar(QWidget *parent = nullptr, int count = 0);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_NAVIGATIONBAR_H
