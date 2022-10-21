// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VIEWSACTIONGROUP_H
#define ANTIQUACRM_VIEWSACTIONGROUP_H

#include <QActionGroup>
#include <QSignalMapper>
#include <QWidget>

class ViewsActionGroup : public QActionGroup {
  Q_OBJECT

private:
  QSignalMapper *m_mapper;

Q_SIGNALS:
  void sendSelectView(const QString &viewName);

public:
  explicit ViewsActionGroup(QObject *parent = nullptr);
  bool loadDataset();
};

#endif // ANTIQUACRM_VIEWSACTIONGROUP_H
