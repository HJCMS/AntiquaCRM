// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef SALUTATIONBOX_UTILS_H
#define SALUTATIONBOX_UTILS_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

#include <UtilsMain>

class SalutationBox : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_comboBox;
  QLineEdit *m_edit;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();

public:
  explicit SalutationBox(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // SALUTATIONBOX_UTILS_H
