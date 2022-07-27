// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CONDITIONFIELD_UTILS_H
#define CONDITIONFIELD_UTILS_H

#include <QObject>
#include <QWidget>
#include <QWheelEvent>
#include <QComboBox>

#include <UtilsMain>

class ConditionBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  explicit ConditionBox(QWidget * parent = nullptr);
};

class ConditionField final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ConditionBox *m_box;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit ConditionField(QWidget * parent = nullptr);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // CONDITIONFIELD_UTILS_H
