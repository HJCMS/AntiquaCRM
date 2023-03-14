// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef RETURN_NAVIGATION_H
#define RETURN_NAVIGATION_H

#include <QDialogButtonBox>
#include <QIcon>
#include <QObject>
#include <QPushButton>
#include <QWidget>

/**
 * @ingroup _returnorder
 * @brief The ReturnNavigation class
 */
class ReturnNavigation final : public QDialogButtonBox {
  Q_OBJECT

private:
  QPushButton *m_save;
  QPushButton *m_apply;
  QPushButton *m_cancel;
  const QIcon setIcon(const QString &);

Q_SIGNALS:
  void sendSave();

public Q_SLOTS:
  void enableSaveButton(bool b = false);

public:
  explicit ReturnNavigation(QWidget *parent = nullptr);
};

#endif // RETURN_NAVIGATION_H
