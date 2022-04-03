// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_H
#define INVENTORY_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtGui/QShowEvent>
#include <QtWidgets/QTabWidget>

/**
   @brief The Inventory class
   Elternklasse für alle Inventar Tabulaturen.
 */
class Inventory : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")
  Q_PROPERTY(int TabIndex READ getTabIndex CONSTANT)
  Q_PROPERTY(QString TabTitle READ tabTitle WRITE setTabTitle CONSTANT REQUIRED)

protected:
  int TabIndex;

protected Q_SLOTS:
  void showEvent(QShowEvent *);

public:
  explicit Inventory(int index, QTabWidget *parent = nullptr);

  /**
     @brief getTabIndex
     @return currentTabIndex()
   */
  int getTabIndex();

  /**
     @brief tabTitle
     @return windowTitle
   */
  const QString tabTitle();

  /**
     @brief setTabTitle - e.g. windowTitle
   */
  void setTabTitle(const QString &);
};

#endif // INVENTORY_H
