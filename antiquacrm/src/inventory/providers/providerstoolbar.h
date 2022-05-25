// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_TOOLBAR_H
#define INVENTORY_PROVIDERS_TOOLBAR_H

#include <QAction>
#include <QFrame>
#include <QLabel>
#include <QObject>
#include <QPushButton>
#include <QToolBar>
#include <QWidget>

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
  QPushButton *btn_customer;
  QPushButton *btn_order;
  QPushButton *btn_refresh;
  /**
   * @brief Seiten Navigator
   */
  ProvidersPager *pager;

Q_SIGNALS:
  void s_customerView();
  void s_createOrder();
  void s_refresh();

public Q_SLOTS:
  void statusMessage(const QString &);
  /**
   * @brief Erstellen Knopf Ein/Ausschalten
   */
  void enableOrderButton(bool);

public:
  explicit ProvidersToolBar(QWidget *parent = nullptr);

  /**
   * @brief Dienstleisterseite aufrufen
   * @param title
   */
  void enterPage(const QString &title);
};

#endif // INVENTORY_PROVIDERS_TOOLBAR_H
