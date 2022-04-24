// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOLBOX_UTILS_H
#define BOOLBOX_UTILS_H

#include <QtCore/QObject>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QWidget>

#include <UtilsMain>

class BoolBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  QCheckBox *m_checkBox;

private Q_SLOTS:
  void itemChanged(int);

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setChecked(bool);

public:
  explicit BoolBox(QWidget *parent = nullptr);
  Q_INVOKABLE bool hasModified();
  bool isChecked();
  void setValue(const QVariant &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // BOOLBOX_UTILS_H
