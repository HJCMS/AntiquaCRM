// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"

#include <QComboBox>
#include <QLabel>
#include <QObject>
#include <QSplitter>
#include <QPushButton>
#include <QStackedWidget>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>

class Buchfreund;

class ProvidersToolBar : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_comboBox;
  QPushButton *btn_order;

Q_SIGNALS:
  void s_createOrder();
  void s_refresh();

public:
  explicit ProvidersToolBar(QWidget *parent = nullptr);
  void addItems(const QStringList &list);
  void setIndex(const QString &title);
  void enableOrderButton(bool);
};

/**
 * @group Providers
 * @class InventoryProviders
 * Primäre Klasse für das Auftrags System
 */
class InventoryProviders : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int costumerId = -1;

  ProvidersToolBar *m_toolBar;

  /**
    @brief Fenster gruppierung
  */
  QStackedWidget *m_stackedWidget;

  /**
   * @brief Page0
   */
  Buchfreund *m_buchfreund;

  void openEditor(const QString &){/* TODO */};

private Q_SLOTS:
  void searchConvert(const QString &search) { /* TODO */
    Q_UNUSED(search)
  };

  /**
    @brief Methode für Suchknopf
   */
  void searchConvert(){/* TODO */};

  /**
    @brief Öffne Tabellenansicht
    Wird mit SIGNAL s_leaveEditor aufgerufen.
    @note Standard ist Tabellenansicht "Page 1"
  */
  void openTableView(){/* TODO */};

  void setButtonState(int);

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryProviders(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_H
