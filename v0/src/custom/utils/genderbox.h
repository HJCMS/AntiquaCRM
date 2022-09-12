// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef GENDERBOX_UTILS_H
#define GENDERBOX_UTILS_H

#include <QComboBox>
#include <QHash>
#include <QLabel>
#include <QObject>
#include <QWidget>
// Personal
#include <UtilsMain>

class Gender {
private:
  QHash<int, QString> p_data;

public:
  explicit Gender();
  int size();
  const QString value(int);
  const QStringList all();
  const QString formOfAddress(const QString &search);
  static const QHash<int, QString> genderData();
  static int indexByString(const QString &search);
};

class GenderBox final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_value = 0;
  QLabel *m_info;
  QComboBox *m_comboBox;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit GenderBox(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // GENDERBOX_UTILS_H
