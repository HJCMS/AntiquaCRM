// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_IMAGE_VIEWTOOLBAR_H
#define ANTIQUACRM_IMAGE_VIEWTOOLBAR_H

#include <AGlobal>
#include <QAction>
#include <QDir>
#include <QFrame>
#include <QObject>
#include <QPushButton>
#include <QWidget>

namespace AntiquaCRM {

class ALineEdit;

/**
 * @class ImageViewToolBar
 * @brief Image Import Actions bar
 * @ingroup ImageWidgets
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
   * @brief Find XDG .config/user-dirs.dirs and search ...
   * Search additional XDG_*_DIR, not supported by Qt*!
   */
  const QString xdgUserDir(const QString &) const;

  /**
   * TreeView Actions
   */
  QPushButton *btn_targets;

private Q_SLOTS:
  void searchArticleImage();
  void prepareTargetChange();

Q_SIGNALS:
  void sendReset();
  void sendCutting();
  void sendRotate();
  void sendAdjust();
  void sendChangeTarget(const QDir &);
  void sendSelectArticle(const QString &);

public:
  explicit ImageViewToolBar(QWidget *parent = nullptr);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_IMAGE_VIEWTOOLBAR_H
