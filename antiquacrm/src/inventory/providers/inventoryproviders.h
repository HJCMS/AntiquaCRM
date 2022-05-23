// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"
#include <AntiquaCRM>

#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QAction>
#include <QWidget>

class Buchfreund;

class ProvidersPager : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int page READ currentPage WRITE setPage NOTIFY pageEntered)

private:
  int page = 0;
  QHash<int,QString> pages;
  QAction *btnLast;
  QLabel *current;
  QAction *btnNext;

Q_SIGNALS:
  void pageEntered(int);
  void pageChanged(const QString &);

public Q_SLOTS:
  void next(bool);
  void last(bool);

public:
  explicit ProvidersPager(QWidget *parent = nullptr);
  /**
   * @brief Eine Dienstleister Seiet einfügen
   */
  void addPages(const QStringList &list);

  void setPage(int index);

  int findPage(const QString &);

  int currentPage();
};

/**
 * @brief Einfacher ToolBar für die Navigation
 */
class ProvidersToolBar : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QLabel *m_status;
  QPushButton *btn_order;
  QPushButton *btn_refresh;

Q_SIGNALS:
  void s_createOrder();
  void s_refresh();

public Q_SLOTS:
  void statusMessage(const QString &);

public:
  /**
   * @brief Seiten Navigator
   */
  ProvidersPager *pager;

  explicit ProvidersToolBar(QWidget *parent = nullptr);

  /**
   * @brief Dienstleisterseite aufrufen
   * @param title
   */
  void enterPage(const QString &title);

  /**
   * @brief Erstellen Knopf Ein/Ausschalten
   */
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
   * @brief Schnittstelle zu "buchfreund.de"
   * Die Kommunikation erfolgt über JSON Fifo
   *
   */
  Buchfreund *bfProvider;

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
