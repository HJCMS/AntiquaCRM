// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILING_MAILBODY_H
#define MAILING_MAILBODY_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

/**
 * @brief The MailBody class
 * @ingroup AntiquaMailing
 */
class MailBody final : public QTextEdit {
  Q_OBJECT

public Q_SLOTS:
  void setFocus();

public:
  explicit MailBody(QWidget *parent = nullptr);
};

#endif // MAILING_MAILBODY_H
