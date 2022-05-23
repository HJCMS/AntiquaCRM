// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CANCELLATION_UTILS_H
#define CANCELLATION_UTILS_H

#include <QComboBox>
#include <QLabel>
#include <QObject>

#include <UtilsMain>

class Cancellation final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  enum Mode { NUMERIC = 1, TEXT = 2 };
  Q_ENUM(Mode);

private:
  Cancellation::Mode p_mode = Cancellation::NUMERIC;
  QLabel *m_info;
  QComboBox *m_box;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE virtual void reset();
  void setFocus();

public:
  explicit Cancellation(QWidget *parent = nullptr,
                        Cancellation::Mode mode = Cancellation::NUMERIC);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // CANCELLATION_UTILS_H
