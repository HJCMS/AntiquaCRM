// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTINFOWIDGET_H
#define ANTIQUACRM_UTILS_ABOUTINFOWIDGET_H

#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QTextEdit>
#include <QTextBrowser>
#include <QUrl>
#include <QWidget>

/**
 * @ingroup _aboutdialogs
 * @brief Abstract Info Widget for About pages
 */
class InfoWidget : public QWidget {
  Q_OBJECT

private:
  QLabel *lb_icon;
  QLabel *lb_title;
  QTextBrowser *lb_text;
  QLabel *lb_homepage;

protected:
  QGridLayout *layout;
  void setInfoIcon(const QIcon &icon);
  void setInfoTitle(const QString &title);
  void setInfoText(const QString &rc_basename);
  void setHomepage(const QUrl &url);

public:
  explicit InfoWidget(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_ABOUTINFOWIDGET_H
