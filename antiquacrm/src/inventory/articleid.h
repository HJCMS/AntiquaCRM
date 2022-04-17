// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ARTICLEID_H
#define ARTICLEID_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

class ArticleID : public QLineEdit {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(bool required READ isRequired WRITE setRequired NOTIFY requireChanged);

private:
  bool required = false;

Q_SIGNALS:
  void requireChanged();

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  Q_INVOKABLE void setModified(bool b);

public:
  explicit ArticleID(QWidget *parent = nullptr);
  void setRequired(bool b);
  bool isRequired();
  Q_INVOKABLE bool hasModified();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // ARTICLEID_H
