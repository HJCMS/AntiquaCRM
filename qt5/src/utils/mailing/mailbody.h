// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef MAILING_MAILBODY_H
#define MAILING_MAILBODY_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @ingroup _mailing
 * @brief eMail Body class
 */
class MailBody final : public QTextEdit {
  Q_OBJECT

public Q_SLOTS:
  void setFocus();

public:
  explicit MailBody(QWidget *parent = nullptr);
};

#endif // MAILING_MAILBODY_H
