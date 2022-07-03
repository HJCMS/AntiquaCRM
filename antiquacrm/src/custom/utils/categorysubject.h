// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYSUBJECT_UTILS_H
#define CATEGORYSUBJECT_UTILS_H

#include <QComboBox>
#include <QLineEdit>
#include <QObject>
#include <QWheelEvent>
#include <QWidget>

#include <UtilsMain>

class CategorySubjectBox final : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void wheelEvent(QWheelEvent *e) override{
      /* Disable wheel actions to prevent invalid inputs! */
  };

public:
  CategorySubjectBox(QWidget *parent = nullptr);
};

class CategorySubject : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  CategorySubjectBox *m_box;
  QLineEdit *m_search;

private Q_SLOTS:
  void subjectChanged(int);
  void searchChanged(const QString &);
  void openHelperDialog();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setFocus();
  void setValue(const QVariant &val);
  void findIndex(const QString &match);

public:
  explicit CategorySubject(QWidget *parent = nullptr);
  void loadDataset();
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // CATEGORYSUBJECT_UTILS_H
