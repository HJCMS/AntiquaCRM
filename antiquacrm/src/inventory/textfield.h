// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class TextField : public QTextEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool modified;

private Q_SLOTS:
  void dataChanged();

public Q_SLOTS:
  void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit TextField(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
};

#endif // TEXTFIELD_H
