// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABSTATUSBAR_H
#define ANTIQUACRM_TABSTATUSBAR_H

#include <QFrame>
#include <QIcon>
#include <QMap>
#include <QMenu>
#include <QPushButton>
#include <QStatusBar>

/**
 * @brief The TabStatusBar class
 * @code
 *  Layout:
 *    MessageLabel | QFrame | Reload Button
 * @endcode
 */
class TabStatusBar : public QStatusBar {
  Q_OBJECT

public:
  enum History {
    Today = 1,
    Yesterday = 2,
    LastWeek = 3,
    LastMonth = 4,
    ThisWeek = 5,
    ThisMonth = 6,
    ThisYear = 7,
    NOIMAGE = 8
  };
  explicit TabStatusBar(QWidget *parent = nullptr);

  /**
   * @brief Erstellen Knopf abfragen
   */
  virtual bool isCreateButtonEnabled() = 0;

  /**
   * @brief Menüeinträge mit den TabStatusBar::History werten!
   */
  static const QMap<TabStatusBar::History, QString> historyItems();

private:
  QPushButton *btn_refresh;

protected:
  /**
   * @brief Search only With Stock
   */
  bool SearchWithStock = false;

  /**
   * @brief Rahmen für eigene Knöpfe und Menüs
   */
  QFrame *m_frame;

  /**
   * @brief Optionaler Erstellen Knopf
   */
  QPushButton *btn_create = nullptr;

  /**
   * @brief Eigener Verlaufsknopf
   */
  QPushButton *btn_history = nullptr;

  /**
   * @brief Erstellt aus @ref historyItems() Menüeinträge
   * @note  Einträge lösen den SLOT @ref setHistoryAction(int) aus!
   */
  void setHistoryActionMenu(QPushButton *parent);

  /**
   * @brief Helferlein für Icon zuweisung
   */
  const QIcon getIcon(const QString &name) const;

protected Q_SLOTS:
  /**
   * @brief Auswahl von Menüeinträgen verarbeiten!
   * @param Es wird "TabStatusBar::History" übergeben.
   * @note Wegen SignalMapper ist der Parameter ein INT!
   */
  virtual void setHistoryAction(int) = 0;

Q_SIGNALS:
  void sendHistoryQuery(const QString &query);
  void sendCreateEntry();
  void sendReloadView();

public Q_SLOTS:
  /**
   * @brief With „Stock“ search enabled or not!
   */
  void setStockEnabled(bool);

  /**
   * @brief Erstellen Knopf aktivieren/deaktivieren
   */
  virtual void setCreateButtonEnabled(bool) = 0;
};

Q_DECLARE_METATYPE(TabStatusBar::History)

#endif // ANTIQUACRM_TABSTATUSBAR_H
