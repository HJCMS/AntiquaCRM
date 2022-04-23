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
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;
  bool modified;

private Q_SLOTS:
  void dataChanged();

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  Q_INVOKABLE void setModified(bool b = true);
  Q_INVOKABLE void reset();

public:
  explicit TextField(QWidget *parent = nullptr);
  void setRequired(bool b);
  bool isRequired();
  void setValue(const QVariant &);
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // TEXTFIELD_H
