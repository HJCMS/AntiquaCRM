// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_@CPPNAME@_H
#define ANTIQUACRM_@CPPNAME@_H

#include <AGlobal>
#include <AntiquaInputEdit>

class @CLASSNAME@ final : public InputEdit {
  Q_OBJECT

private:
  // Qt classes
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit @CLASSNAME@(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_@CPPNAME@_H
