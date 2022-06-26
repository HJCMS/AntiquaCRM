// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICE_H
#define DELIVERSERVICE_H

#include <QObject>
#include <QWidget>
#include <QMap>

class DeliverService : public QWidget
{
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QWidget *m_edit;

private Q_SLOTS:

Q_SIGNALS:

public:
  explicit DeliverService(QWidget *parent = nullptr);
  void addDeliverServiceEntry(const QMap<QString,QVariant> entry);
};

#endif // DELIVERSERVICE_H
