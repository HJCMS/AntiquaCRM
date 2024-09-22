// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_KEYWORDLABEL_H
#define ANTIQUACRM_PRIVATE_KEYWORDLABEL_H

#include <AntiquaCRM>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM
{

class ANTIQUACRM_LIBRARY KeywordLabel final : public QFrame {
  Q_OBJECT

private:
  QLabel* m_label;

Q_SIGNALS:
  void aboutToRemove();

public Q_SLOTS:
  void setText(const QString& keyword);

public:
  explicit KeywordLabel(QWidget* parent = nullptr);
  explicit KeywordLabel(const QString& keyword, QWidget* parent = nullptr);
  const QString text();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_KEYWORDLABEL_H
