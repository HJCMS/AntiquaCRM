// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DESIGNATIONS_LISTITEM_H
#define DESIGNATIONS_LISTITEM_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QWidget>

class DesignationItem : public QListWidgetItem {
private:
  QString p_description;

public:
  DesignationItem(const QString &title, QListWidget *parent = nullptr);
  void setDescription(const QString &);
  const QString getDescription();
};

#endif // DESIGNATIONS_LISTITEM_H
