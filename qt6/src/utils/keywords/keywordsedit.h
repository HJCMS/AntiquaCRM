// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_KEYWORDS_EDIT_H
#define ANTIQUACRM_UTILS_KEYWORDS_EDIT_H

#include <AntiquaWidgets>
#include <QSqlRecord>
#include <QWidget>

class KeywordsEdit final : public QWidget {
  Q_OBJECT

private:
  QSqlRecord p_record;
  AntiquaCRM::NumEdit *m_id;
  AntiquaCRM::TextLine *m_keyword;
  AntiquaCRM::BoolBox *m_usage;

private Q_SLOTS:
  void keywordChanged();

Q_SIGNALS:
  void search(const QString &);

public Q_SLOTS:
  void clear();
  void setData(const QSqlRecord &);
  void changeId(qint64);

public:
  explicit KeywordsEdit(QWidget *parent = nullptr);
  const QSqlRecord getData(const QString &tableName);
};

#endif // ANTIQUACRM_UTILS_KEYWORDS_EDIT_H
