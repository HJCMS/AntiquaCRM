// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILFORWARDDIALOG_UTILS_H
#define MAILFORWARDDIALOG_UTILS_H

#include <QDialog>
#include <QObject>
#include <QWidget>

class MailForwardDialog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit MailForwardDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // MAILFORWARDDIALOG_UTILS_H
