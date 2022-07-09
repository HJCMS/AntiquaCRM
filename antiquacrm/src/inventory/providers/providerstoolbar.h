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
#include <QTimerEvent>
#include <QWidget>

/**
 * @brief Einfacher ToolBar für die Navigation
 * @class ProvidersToolBar
 */
class ProvidersToolBar : public QFrame {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int timerId;
  int counter = 300;
  QLabel *m_status;
  QPushButton *btn_customer;
  QPushButton *btn_order;
  QPushButton *btn_refresh;

  const QString timeString() const;

protected:
  void timerEvent(QTimerEvent *event);

Q_SIGNALS:
  void s_customerAction();
  void s_createOrder();
  void s_refresh();

public Q_SLOTS:
  /**
   * @brief Status Meldungen
   */
  void statusMessage(const QString &);

  /**
   * @brief Warnmeldungen
   */
  void warningMessage(const QString &);

  /**
   * @brief Erstellen Knopf Ein/Ausschalten
   */
  void enableOrderButton(bool b = false);

  /**
   * @brief Bestehende Status Meldungen leeren.
   * Verhindern das Nachrichten beim Tabwechsel vom alten Tab stehen!
   */
  inline void clearStatusMessage() { m_status->clear(); };

public:
  explicit ProvidersToolBar(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_TOOLBAR_H
