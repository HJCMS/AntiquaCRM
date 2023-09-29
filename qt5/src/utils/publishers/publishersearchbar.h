// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_SEARCHBAR_H
#define PUBLISHERS_SEARCHBAR_H

#include <QLineEdit>
#include <QObject>
#include <QToolBar>

class PublisherSearchbar final : public QToolBar {
  Q_OBJECT

private:
  QLineEdit *m_searchLine;

private Q_SLOTS:
  void prepareSearch(const QString &search = QString());

Q_SIGNALS:
  void searchPublisher(const QString &);

public:
  explicit PublisherSearchbar(QWidget *parent = nullptr);
};

#endif // PUBLISHERS_SEARCHBAR_H
