// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_LABEL_H
#define KEYWORD_LABEL_H

#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QToolButton>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Label
 */
class KeywordLabel final : public QFrame {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

private:
  QLabel *m_lable;
  QToolButton *m_close;
  const QString styleSheet() const;

Q_SIGNALS:
  void textChanged();
  void aboutToRemove();

public Q_SLOTS:
  void setText(const QString &);

public:
  explicit KeywordLabel(QWidget *parent = nullptr);
  explicit KeywordLabel(const QString &text, QWidget *parent = nullptr);
  const QString text();
};

#endif // KEYWORD_LABEL_H
