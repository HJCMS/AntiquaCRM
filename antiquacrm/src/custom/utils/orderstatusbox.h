// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSTATUSBOX_H
#define ORDERSTATUSBOX_H

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QObject>
#include <QWidget>

#include <UtilsMain>

class OrderStatusBox final : public UtilsMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int p_value = 0;
  QLabel *m_info;
  QComboBox *m_box;

private Q_SLOTS:
  void indexChanged(int);

public Q_SLOTS:
  void setValue(const QVariant &);
  Q_INVOKABLE void reset();
  void setFocus();

public:
  OrderStatusBox(QWidget *parent = nullptr);
  int findIndex(const QString &);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

class StatusDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  OrderStatusBox *m_statusBox;

public:
  explicit StatusDialog(QWidget *parent = nullptr);
  StatusDialog(qint64 index, QWidget *parent = nullptr);
  StatusDialog(const QString &current, QWidget *parent = nullptr);
  int index();
};

#endif // ORDERSTATUSBOX_H
