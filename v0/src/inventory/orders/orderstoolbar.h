// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERS_TOOLBAR_H
#define INVENTORY_ORDERS_TOOLBAR_H

#include <QComboBox>
#include <QFrame>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QToolButton>
#include <QWidget>

class OrdersMenuButton;

class OrdersToolBar : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(OrdersToolBar::Search section READ getSearchSection WRITE
                 setSearchSection NOTIFY sendSectionChanged)

public:
  enum Search {
    SEARCH_ORDER_ID = 0x1,
    SEARCH_CUSTOMER = 0x2,
    SEARCH_PROVIDER = 0x3
  };
  explicit OrdersToolBar(QWidget *parent = nullptr);
  OrdersToolBar::Search getSearchSection();
  const QString getSearchLineEdit();
  void setSearchSection(OrdersToolBar::Search sec);

private:
  OrdersToolBar::Search section = Search::SEARCH_ORDER_ID;
  QString p_searchText;
  QToolButton *m_infoBtn;
  QComboBox *m_searchSection;
  QLineEdit *m_searchLineEdit;
  OrdersMenuButton *m_menuButton;
  QPushButton *btn_refresh;

private Q_SLOTS:
  void searchSectionChanged(int index);
  void searchTextChanged(const QString &str);

Q_SIGNALS:
  void sendSectionChanged(int);
  void sendSearchText(const QString &);
  void sendRefreshView();
  void sendDefaultView();
  void sendCustomQuery(const QString &query);
};
Q_DECLARE_METATYPE(OrdersToolBar::Search);

#endif // INVENTORY_ORDERS_TOOLBAR_H
