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
#include <QWidget>

class ArticleDescWordsTable;

class ArticleDescWordsWidget final : public QWidget {
  Q_OBJECT

private:
  ArticleDescWordsTable* m_editTable;
  AntiquaCRM::SerialId* m_id;
  AntiquaCRM::AComboBox* m_type;
  AntiquaCRM::SelectLanguage* m_lang;
  AntiquaCRM::TextLine* m_title;
  AntiquaCRM::TextField* m_data;

Q_SIGNALS:
  void sendCloseDialog(bool);

public Q_SLOTS:
  void sqlCommit();

public:
  explicit ArticleDescWordsWidget(QWidget* parent = nullptr);
  bool sqlQuery();
};

#endif // ANTIQUACRM_UTILS_ARTICLEDESCWORDSWIDGET_H
