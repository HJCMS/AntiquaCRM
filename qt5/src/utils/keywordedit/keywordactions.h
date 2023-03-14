// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef KEYWORD_ACTIONS_H
#define KEYWORD_ACTIONS_H

#include <QFrame>
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

/**
 * @ingroup _keywordedit
 * @brief Keywords Edit Actions frame
 */
class KeywordActions : public QFrame {
  Q_OBJECT

private:
  QLabel *m_messages;
  QPushButton *m_btnQuit;

Q_SIGNALS:
  void sendQuit();

public Q_SLOTS:
  void statusMessage(const QString &);

public:
  explicit KeywordActions(QWidget *parent = nullptr);
};

#endif // KEYWORD_ACTIONS_H
