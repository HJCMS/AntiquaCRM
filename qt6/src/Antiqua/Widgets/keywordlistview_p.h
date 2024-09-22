// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_KEYWORDEDITOR_LISTVIEW_H
#define ANTIQUACRM_KEYWORDEDITOR_LISTVIEW_H

#include <AntiquaCRM>
#include <QHBoxLayout>
#include <QObject>
#include <QStringList>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY KeywordListView final : public QWidget {
  Q_OBJECT

private:
  QHBoxLayout *layout;
  QStringList p_uniqList;

Q_SIGNALS:
  void keywordsChanged();

public Q_SLOTS:
  void setKeywordList(const QStringList &);
  void insertKeyword(const QString &);
  void removeKeyword();
  void clearKeywords();

public:
  explicit KeywordListView(QWidget *parent = nullptr);
  const QString getKeywords();
  const QString getToolTip();
  bool isValid();
  int curLength();
  static int minLength();
  static int maxLength();
};

} // namespace AntiquaCRM

#endif
