// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SIZEEDIT_UTILS_H
#define SIZEEDIT_UTILS_H

#include <UtilsMain>

#include <QObject>
#include <QLabel>
#include <QSpinBox>

class SizeEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel   *m_info;
  QSpinBox *m_width;
  QSpinBox *m_height;

private Q_SLOTS:
  void sizeChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &);

public:
  SizeEdit(QWidget *parent = nullptr);
  const QVariant value();
  void setMinimum(int width, int height);
  void setMaximum(int width, int height);
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // SIZEEDIT_UTILS_H
