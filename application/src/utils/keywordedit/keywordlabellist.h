// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LABEL_LIST_H
#define KEYWORD_LABEL_LIST_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QWidget>

class KeywordLabel;

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Labels list
 */
class KeywordLabelList : public QFrame {
  Q_OBJECT

private:
  QHBoxLayout *m_layout;
  QStringList p_uniqList;

private Q_SLOTS:
  void removeKeyword();

Q_SIGNALS:
  void sendModified(bool);

public Q_SLOTS:
  void clear();
  void addKeyword(const QString &keyword);

public:
  explicit KeywordLabelList(QWidget *parent = nullptr);
  void addKeywords(const QStringList &keywords);
  const QStringList keywords();
};

#endif // KEYWORD_LABEL_LIST_H
