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
 * @brief Databse Image previewer
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageToolBar final : public QToolBar {
  Q_OBJECT

private:
  qint64 articleId = -1;
  QPushButton *m_btnOpen;
  QPushButton *m_btnRemove;

private Q_SLOTS:
  void checkOpen();
  void checkRemove();

Q_SIGNALS:
  void sendOpenImage();
  void sendDeleteImage(qint64);

public Q_SLOTS:
  void setArticleId(qint64);
  void setRestoreState();

public:
  explicit ImageToolBar(QWidget *parent = nullptr);
  qint64 getArticleId();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGETOOLBAR_H
