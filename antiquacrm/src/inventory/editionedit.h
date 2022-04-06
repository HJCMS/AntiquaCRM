// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITIONEDIT_H
#define EDITIONEDIT_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

class EditionEdit : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public Q_SLOTS:
  void setValue(const QVariant &);

public:
  explicit EditionEdit(QWidget *parent = nullptr);
  const QVariant value();
};

#endif // EDITIONEDIT_H
