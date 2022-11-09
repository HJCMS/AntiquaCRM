// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERSTREEVIEW_H
#define ANTIQUACRM_PROVIDERSTREEVIEW_H

#include <QDateTime>
#include <QTimerEvent>
#include <QLabel>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class ProvidersTreeView final : public QTreeWidget {
  Q_OBJECT

private:
  struct TreeOrderItem {
    QString id;
    QDateTime datetime;
    QString buyer;
    int status;
  };
  int timerId;
  int counter;
  const int resetCounter = 300; /**< steps in Seconds */
  const QString setDateString(const QDateTime &dt) const;
  QTreeWidgetItem *getParent(const QString &name);
  QTreeWidgetItem *getChild(const QString &provider, const QString &id);
  void timerEvent(QTimerEvent *event) override;
  void addProvider(const QString &provider);

private Q_SLOTS:
  void itemSelected(QTreeWidgetItem *, int);
  void updateOrderStatus(QTreeWidgetItem *item, int status);

  /**
   * @brief Dienstleister liste leeren!
   */
  void clearProvider(const QString &provider);

  /**
   * @brief Eine Bestellung einfügen
   */
  void addOrder(const QString &pro, const ProvidersTreeView::TreeOrderItem &data);

Q_SIGNALS:
  /**
   * @brief Auswahl getroffen
   * @param provider  - Dienstleister
   * @param id        - Bestellnummer
   */
  void sendQueryOrder(const QString &provider, const QString &id);

public Q_SLOTS:
  /**
   * @brief Eintrag entfernen
   * @param provider - Zeiger auf Parent
   * @param id       - Bestellung
   */
  void removeOrder(const QString &provider, const QString &id);

  /**
   * @brief Sort all Items
   */
  void sortAndResize();

  /**
   * @brief load or Update Tree content
   */
  void loadUpdate();

public:
  explicit ProvidersTreeView(QWidget *parent = nullptr);

  /**
   * @brief Doppelte Einträge vermeiden
   * @param provider - Dienstleister
   * @param id       - Bestellnummer
   */
  bool exists(const QString &provider, const QString &id);

  /**
   * @brief Darstellung der Einträge auf Bestand anpassen!
   * @param provider
   * @param orderId
   * @param status
   */
  void updateItemStatus(const QString &provider, const QString &orderId,
                        int status = 0);

  int ordersCount();

  ~ProvidersTreeView();
};

#endif // ANTIQUACRM_PROVIDERSTREEVIEW_H
