// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGETOOLBAR_H
#define ANTIQUACRM_WIDGETS_IMAGETOOLBAR_H

#include <AntiquaInput>
#include <QObject>
#include <QPushButton>
#include <QToolBar>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class ImageToolBar
 * @brief Source image and Database Thumbnails Actions Toolbar
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageToolBar final : public QToolBar {
  Q_OBJECT

private:
  /**
   * @brief Article number
   */
  qint64 articleId = -1;

  /**
   * @brief Open Import Source Image dialog button
   */
  QPushButton *m_btnOpen;

  /**
   * @brief Remove Thumbnail button
   */
  QPushButton *m_btnRemove;

private Q_SLOTS:
  /**
   * @brief Prepare and emit sendDeleteImage signal.
   * Ensures an Article number exists before issuing the delete signal.
   */
  void beforeRemove();

Q_SIGNALS:
  /**
   * @brief Signal for trigger Open Image Dialog
   */
  void sendOpenImage();

  /**
   * @brief Delete image was clicked
   * @note requires a valid Article number
   */
  void sendDeleteImage(qint64);

public Q_SLOTS:
  /**
   * @brief set Article number
   * If article number is valid the remove thumbnail button will enabled
   */
  void setArticleId(qint64);

  /**
   * @brief Restore to default state
   * Reset Article number and disable remove thumbnail button!
   */
  void setRestoreState();

public:
  /**
   * @param parent - parent object
   */
  explicit ImageToolBar(QWidget *parent = nullptr);

  /**
   * @brief get current add Article number
   */
  qint64 getArticleId();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGETOOLBAR_H
