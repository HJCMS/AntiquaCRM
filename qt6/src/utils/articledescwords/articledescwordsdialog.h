// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ARTICLEDESCWORDS_H
#define ANTIQUACRM_UTILS_ARTICLEDESCWORDS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QStatusBar>

class ArticleDescWordsWidget;

class ArticleDescWordsDialog final : public QDialog {
  Q_OBJECT

private:
  ArticleDescWordsWidget* m_editWidget;
  QDialogButtonBox* m_btnBox;
  QStatusBar* m_statusBar;

  void keyPressEvent(QKeyEvent*) override;
  bool event(QEvent*) override;

private Q_SLOTS:
  void closeDialog(bool b = false);

public:
  explicit ArticleDescWordsDialog(QWidget* parent = nullptr);
  int exec() override;
};

#endif // ANTIQUACRM_UTILS_ARTICLEDESCWORDS_H
