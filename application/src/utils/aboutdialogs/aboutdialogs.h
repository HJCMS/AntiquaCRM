// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ABOUT_DIALOGS_H
#define ANTIQUACRM_ABOUT_DIALOGS_H

#include <QDialog>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class AboutAntiquaCRM;
class AboutQREncode;
class AboutPostgreSQL;

/**
 * @ingroup utils
 * @brief All About Dialogs
 */
class AboutDialogs final : public QDialog {
  Q_OBJECT

private:
  QStackedWidget *m_pages;
  AboutAntiquaCRM *m_page0;
  AboutQREncode *m_page1;
  AboutPostgreSQL *m_page2;

public:
  enum Page { ANTIQUACRM = 0, QRENCODE = 1, POSTGRESQL = 2 };
  explicit AboutDialogs(QWidget *parent = nullptr);
  int exec() override;
  int exec(AboutDialogs::Page);
  ~AboutDialogs();
};

#endif
