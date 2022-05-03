// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEXTFIELD_UTILS_H
#define TEXTFIELD_UTILS_H

#include <QObject>
#include <QVariant>
#include <QTextEdit>
#include <QWidget>

#include <UtilsMain>

class TextField : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTextEdit *m_edit;

private Q_SLOTS:
  void dataChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit TextField(QWidget *parent = nullptr);
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // TEXTFIELD_UTILS_H
