// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGESELECTER_UTILS_H
#define MESSAGESELECTER_UTILS_H

#include <QObject>
#include <QToolBar>
#include <QWidget>

class MessageSelecter final : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  explicit MessageSelecter(QWidget *parent = nullptr);
};

#endif // MESSAGESELECTER_UTILS_H
