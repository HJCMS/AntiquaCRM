// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef YEAREDIT_UTILS_H
#define YEAREDIT_UTILS_H

#include <QtCore/QObject>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QWidget>

class YearEdit : public QDateEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  const QDate startDate;
  bool required = false;
  bool modified;

private Q_SLOTS:
  void dataChanged(const QDate &);

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  Q_INVOKABLE void setModified(bool b = true);
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit YearEdit(QWidget *parent = nullptr);
  void setRequired(bool b);
  bool isRequired();
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // YEAREDIT_UTILS_H
