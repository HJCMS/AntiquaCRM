// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"
#include <AntiquaCRM>

#include <QAction>
#include <QComboBox>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QStackedWidget>
#include <QTabWidget>
#include <QToolBar>
#include <QWidget>

class Buchfreund;

/**
 * @brief Navigation für Dienstleister Seiten
 * @class ProvidersPager
 */
class ProvidersPager : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int page READ currentPage WRITE setPage NOTIFY pageEntered)

private:
  int page = 0;
  QHash<int, QString> pages;
  QAction *btnLast;
  QLabel *current;
  QAction *btnNext;

Q_SIGNALS:
  /**
   * @brief Seite geändert
   */
  void pageEntered(int);

  /**
   * @brief Auf Seitenname geändert
   */
  void pageChanged(const QString &);

public Q_SLOTS:
  /**
   * @brief Eine Seite weiter
   */
  void next(bool);

  /**
   * @brief Eine Seite zurück
   */
  void last(bool);

public:
  explicit ProvidersPager(QWidget *parent = nullptr);

  /**
   * @brief Eine Dienstleister Seite einfügen
   */
  void addPages(const QStringList &list);

  /**
   * @brief Seite wechseln zu index ...
   */
  void setPage(int index);

  /**
   * @brief Finde Seiten Index
   */
  int findPage(const QString &);

  /**
   * @brief Aktueller Seiten Index
   */
  int currentPage();
};

/**
 * @brief Einfacher ToolBar für die Navigation
 * @class ProvidersToolBar
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
  /**
   * @brief Erstellen Knopf Ein/Ausschalten
   */
  void enableOrderButton(bool);

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
  int customerId = -1;

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

  /**
   * @brief Suche Dienstleister und erstelle Seiten
   */
  void initPages();

  void openEditor(const QString &){/* TODO */};

Q_SIGNALS:
  void openEditCustomer(int cid);

private Q_SLOTS:
  void searchConvert(const QString &search) { /* TODO */
    Q_UNUSED(search)
  };

  /**
   * @brief Methode für Suchknopf
   */
  void searchConvert(){/* hier nicht notwendig */};

  /**
   * @brief Öffne Tabellenansicht
   */
  void openTableView(){/* hier nicht notwendig */};

  /**
   * @brief Anhand der Kundennummer bestimmen
   *  ob der Erstellen Knopf aktiviert wird!
   */
  void sendViewCustomer(int cid);

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryProviders(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_H
