// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SERIALID_H
#define ANTIQUACRM_SERIALID_H

#include <AntiquaInputEdit>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QVariant>

class SerialID final : public InputEdit {
  Q_OBJECT

private:
  QLabel *m_serialLabel;
  void loadDataset();

Q_SIGNALS:
  void sendSerialChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  explicit SerialID(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  void setInfo(const QString &);
  const QString info();
  const QVariant value();
  bool isValid();
  const QString notes();
};

#endif // ANTIQUACRM_SERIALID_H
