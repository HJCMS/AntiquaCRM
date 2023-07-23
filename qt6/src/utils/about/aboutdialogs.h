// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ABOUTDIALOGS_H
#define ANTIQUACRM_UTILS_ABOUTDIALOGS_H

#include <QDialog>
#include <QMenu>
#include <QSignalMapper>
#include <QStackedWidget>
#include <QWidget>

class AboutAntiquaCRM;
class AboutQREncode;
class AboutPostgreSQL;
class AboutDiscId;

class AboutDialogs final : public QDialog {
  Q_OBJECT

private:
  QStackedWidget *m_pages;
  AboutAntiquaCRM *m_page0;
  AboutQREncode *m_page1;
  AboutPostgreSQL *m_page2;
  AboutDiscId *m_page3;

public:
  enum Page {
    ANTIQUACRM = 0, // Antiqua CRM
    QRENCODE = 1,   // QREncode
    POSTGRESQL = 2, // PostgreSQL
    MUSICBRAINZ = 3 // DiscID
  };
  explicit AboutDialogs(QWidget *parent = nullptr);
  int exec() override;
  int exec(AboutDialogs::Page);
};

class AboutMenu final : public QMenu {
  Q_OBJECT

private:
  QSignalMapper *m_mapper;

private Q_SLOTS:
  void openDialog(int page);

public:
  explicit AboutMenu(QWidget *parent = nullptr);
  virtual ~AboutMenu();
};

#endif // ANTIQUACRM_UTILS_ABOUTDIALOGS_H
