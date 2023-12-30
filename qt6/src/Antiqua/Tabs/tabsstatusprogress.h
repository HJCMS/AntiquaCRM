// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_TABS_STATUSPROGRESS_H
#define ANTIQUACRM_TABS_STATUSPROGRESS_H

#include <AGlobal>
#include <QLabel>
#include <QObject>
#include <QProgressBar>
#include <QPropertyAnimation>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY TabsProgressAnimation final
    : public QPropertyAnimation {
  Q_OBJECT

public:
  explicit TabsProgressAnimation(QProgressBar *parent);
};

class ANTIQUACRM_LIBRARY TabsStatusProgress final : public QProgressBar {
  Q_OBJECT

private:
  QLabel *m_info;

public Q_SLOTS:
  /**
   * @brief Status message
   * @param message
   * @param timeout
   */
  void showMessage(const QString &message, int timeout = 0);

  /**
   * @brief Status reset
   */
  void reset();

public:
  explicit TabsStatusProgress(QWidget *parent = nullptr);
  void start(const QString &message);
  virtual ~TabsStatusProgress();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_TABS_STATUSPROGRESS_H
