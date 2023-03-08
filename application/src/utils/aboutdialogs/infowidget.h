// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABOUT_INFOWIDGET_H
#define ANTIQUACRM_ABOUT_INFOWIDGET_H

#include <QGridLayout>
#include <QIcon>
#include <QLabel>
#include <QTextEdit>
#include <QUrl>
#include <QWidget>

/**
 * @ingroup AntiquaAbout
 * @brief Abstract Info Widget for About pages
 */
class InfoWidget : public QWidget {
  Q_OBJECT

private:
  QLabel *lb_icon;
  QLabel *lb_title;
  QTextEdit *lb_text;
  QLabel *lb_homepage;

protected:
  QGridLayout *layout;
  void setInfoIcon(const QIcon &icon);
  void setInfoTitle(const QString &title);
  void setInfoText(const QString &rc_basename);
  void setHomepage(const QUrl &url);

public:
  explicit InfoWidget(QWidget *parent = nullptr);

  /**
   * @brief Set current Info Widget
   */
  virtual void setInfoWidget() = 0;
};

#endif // ANTIQUACRM_ABOUT_INFOWIDGET_H
