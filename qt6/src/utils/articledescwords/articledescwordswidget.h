// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ARTICLEDESCWORDSWIDGET_H
#define ANTIQUACRM_UTILS_ARTICLEDESCWORDSWIDGET_H

#include <AntiquaWidgets>
#include <QModelIndex>
#include <QWidget>

class ArticleDescWordsTable;

class ArticleDescWordsWidget final : public QWidget {
  Q_OBJECT

private:
  ArticleDescWordsTable* m_table;
  AntiquaCRM::SerialId* m_id;
  AntiquaCRM::SectionSelecter* m_type;
  AntiquaCRM::SelectLanguage* m_lang;
  AntiquaCRM::TextLine* m_title;
  AntiquaCRM::TextField* m_data;

private Q_SLOTS:
  void clearInput();
  void openEntry(qint64);
  void deleteEntry(qint64);
  void itemSelected(const QModelIndex&);

Q_SIGNALS:
  void sendStatusMessage(const QString &);

public Q_SLOTS:
  void sqlCommit();

public:
  explicit ArticleDescWordsWidget(QWidget* parent = nullptr);
  bool sqlQuery();
};

#endif // ANTIQUACRM_UTILS_ARTICLEDESCWORDSWIDGET_H
