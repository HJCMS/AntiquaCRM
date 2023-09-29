// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_INPUT_H
#define PUBLISHERS_INPUT_H

#include <QLineEdit>
#include <QObject>
#include <QWidget>

class PublishersInput : public QWidget {
  Q_OBJECT

private:
  QLineEdit *m_publisher;
  QLineEdit *m_location;

  const QString strip(const QString &) const;

private Q_SLOTS:
  void dataChanged();

public Q_SLOTS:
  void setPublisher(const QString &name);
  void setLocation(const QString &location);

public:
  explicit PublishersInput(QWidget *parent = nullptr);
  const QString getPublisher();
  const QString getLocation();
};

#endif // PUBLISHERS_INPUT_H
