// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef URLEDIT_UTILS_H
#define URLEDIT_UTILS_H

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include <UtilsMain>

class UrlEdit : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_label;
  QLineEdit *m_edit;

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit UrlEdit(QWidget *parent = nullptr);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // URLEDIT_UTILS_H
