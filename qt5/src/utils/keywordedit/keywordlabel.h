// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LABEL_H
#define KEYWORD_LABEL_H

#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QWidget>

class KeywordLabelList;

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Label
 */
class KeywordLabel final : public QFrame {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

private:
  QPoint p_dragBegin;
  QLabel *lable;
  const QString styleSheet() const;

Q_SIGNALS:
  void textChanged();
  void aboutToRemove();

public Q_SLOTS:
  void setText(const QString &);

public:
  explicit KeywordLabel(KeywordLabelList *parent = nullptr);
  explicit KeywordLabel(const QString &text,
                        KeywordLabelList *parent = nullptr);
  const QString text();
};

#endif // KEYWORD_LABEL_H
