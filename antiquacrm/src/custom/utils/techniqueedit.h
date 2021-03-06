// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TECHNIQUEEDIT_UTILS_H
#define TECHNIQUEEDIT_UTILS_H

#include <QObject>
#include <QVariant>
#include <QComboBox>
#include <QWidget>

#include <UtilsMain>

class TechniqueEdit final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_comboBox;
  void laodData();

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void loadDataset();
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit TechniqueEdit(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // TECHNIQUEEDIT_UTILS_H
