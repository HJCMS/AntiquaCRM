// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GENDERBOX_H
#define GENDERBOX_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

class GenderBox : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;
  bool modified;

private Q_SLOTS:
  void itemChanged(int);

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit GenderBox(QWidget *parent = nullptr);
  void setRequired(bool b);
  bool isRequired();
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // GENDERBOX_H
