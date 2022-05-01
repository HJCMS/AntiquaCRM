// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSTATUSBOX_H
#define ORDERSTATUSBOX_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

class OrderStatusBox : public QComboBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged)

private:
  int index;

Q_SIGNALS:
  void indexChanged(int);

public Q_SLOTS:
  void setIndex(int);

public:
  OrderStatusBox(QWidget *parent = nullptr);
  int getIndex();
};

class StatusDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann (Undefined)")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  OrderStatusBox *m_box;

public:
  explicit StatusDialog(const QString &current, QWidget *parent = nullptr);
  int index();
};

#endif // ORDERSTATUSBOX_H
