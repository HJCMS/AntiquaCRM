// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABWIDGET_H
#define ANTIQUACRM_TABWIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QJsonObject>
#include <QMainWindow>
#include <QMap>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

class AntiquaTabBar;
class AntiquaSocketServer;

class AntiquaTabWidget final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaSocketServer *m_socket;
  AntiquaTabBar *m_tabBar;
  TabViews *m_views;
  QWidget *m_testing;

  bool createSocketListener();

  bool addInventoryTab(const QString &name);
  inline Inventory *tabWidget(int index) const;
  int indexByName(const QString &name) const;

private Q_SLOTS:
  void setAction(const QJsonObject &obj);
  void addViewsTab(const QString &query);
  void setTabChanged(int index);
  void setTabToClose(int index);
  void setTabToVisit(int index);

Q_SIGNALS:
  void sendStatusMessage(const QString &);

public Q_SLOTS:
  void setShowTab(const QString &tabId);

public:
  explicit AntiquaTabWidget(QMainWindow *parent = nullptr);
  static const QMap<QString, QString> availableTabs();
  ~AntiquaTabWidget();
};

#endif // ANTIQUACRM_TABWIDGET_H
