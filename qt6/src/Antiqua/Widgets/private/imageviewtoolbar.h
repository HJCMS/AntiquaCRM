// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_IMAGEVIEWTOOLBAR_H
#define ANTIQUACRM_PRIVATE_IMAGEVIEWTOOLBAR_H

#include <AGlobal>
#include <QDir>
#include <QAction>
#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @brief Image Import Actions bar
 */
class ANTIQUACRM_LIBRARY ImageViewToolBar final : public QFrame {
  Q_OBJECT

private:
  /**
   * ImageViewer Actions
   */
  QAction *ac_reset;
  QAction *ac_cut;
  QAction *ac_rotate;
  QAction *ac_scale;
  QAction *ac_save;
  QAction *ac_close;

  /**
   * TreeView Actions
   */
  QPushButton *btn_targets;

private Q_SLOTS:
  void prepareTargetChange();

Q_SIGNALS:
  void sendReset();
  void sendCutting();
  void sendRotate();
  void sendAdjust();
  void sendChangeTarget(const QDir &);

public:
  explicit ImageViewToolBar(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_IMAGEVIEWTOOLBAR_H
