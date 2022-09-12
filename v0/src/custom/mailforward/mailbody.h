// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MAILBODY_UTILS_H
#define MAILBODY_UTILS_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class MailBody : public QTextEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit MailBody(QWidget *parent = nullptr);
};

#endif // MAILBODY_UTILS_H
