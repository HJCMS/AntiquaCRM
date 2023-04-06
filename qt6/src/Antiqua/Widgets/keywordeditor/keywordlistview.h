// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_KEYWORDEDITOR_LISTVIEW_H
#define ANTIQUACRM_KEYWORDEDITOR_LISTVIEW_H

#include <AGlobal>
#include <QHBoxLayout>
#include <QObject>
#include <QStringList>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY KeywordListView final : public QWidget {
  Q_OBJECT

private:
  int maxLength = 60;
  QHBoxLayout *layout;
  QStringList p_uniqList;

  void updateToolTip();

Q_SIGNALS:
  void valueChanged();

public Q_SLOTS:
  void setKeywordList(const QStringList &);
  void insertKeyword(const QString &);
  void removeKeyword();
  void clearKeywords();

public:
  explicit KeywordListView(QWidget *parent = nullptr);
  const QString getKeywords();
  qsizetype length();
};

} // namespace AntiquaCRM

#endif
