// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef VATSELECTER_UTILS_H
#define VATSELECTER_UTILS_H

#include <QComboBox>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class VATSelecter final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_box;

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &);

public:
  explicit VATSelecter(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // VATSELECTER_UTILS_H
