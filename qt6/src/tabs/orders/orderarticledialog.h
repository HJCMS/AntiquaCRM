// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERARTICLEDIALOG_H
#define ANTIQUACRM_PLUGIN_ORDERARTICLEDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QWidget>

class ANTIQUACRM_LIBRARY OrderArticleDialog final : public QDialog {
  Q_OBJECT

private:
  qint64 articleId = -1;
  QString articleInfo = QString();
  QSpinBox *m_searchEdit;
  QPushButton *btn_search;
  QPushButton *btn_apply;
  QDialogButtonBox *btn_box;
  QStatusBar *m_statusBar;
  inline AntiquaCRM::ArticleOrderItem addItem(const QString &key,
                                              const QVariant &value) const;
  bool queryArticle(qint64 aid);

private Q_SLOTS:

  void setSearchArticle();

  /**
   * @brief Such/Einfügen Felder nach dem einfügen leeren.
   * @note OrderArticle wird auch geleert!
   */
  void clearSearchInput();

  /**
   * @brief Erstellt einen Roten Text neben dem Article Einfügen!
   * Wenn der kein Text dann wird die Ausgabe geleert!
   */
  void setInfoMessage(const QString &message);

Q_SIGNALS:
  void searchArticleById(qint64);

public:
  explicit OrderArticleDialog(QWidget *parent = nullptr);
  qint64 getArticle();
  int exec() override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERARTICLEDIALOG_H
