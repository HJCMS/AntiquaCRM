// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_TREEVIEW_H
#define INVENTORY_PROVIDERS_TREEVIEW_H

#include <QDateTime>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

/**
 * @brief Dienstleisterliste anzeigen
 * @class ProvidersTreeView
 * Zeigt die geladenen Dienstleister an.
 */
class ProvidersTreeView : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTreeWidgetItem *getParent(const QString &name);
  QTreeWidgetItem *getChild(const QString &provider, const QString &id);

private Q_SLOTS:
  void itemSelected(QTreeWidgetItem *, int);

public Q_SLOTS:
  /**
   * @brief Eine Bestellung einfügen
   * @param provider - Zeiger auf Parent
   * @param id       - Besteller Id
   * @param dt       - Datum
   */
  void addOrder(const QString &provider, const QString &id,
                const QDateTime &dt);

  /**
   * @brief Eintrag entfernen
   * @param provider - Zeiger auf Parent
   * @param id       - Bestellung
   */
  void removeOrder(const QString &provider, const QString &id);

Q_SIGNALS:
  /**
   * @brief Auswahl getroffen
   * @param provider  - Dienstleister
   * @param id        - Bestellnummer
   */
  void s_queryOrder(const QString &provider, const QString &id);

  /**
   * @brief Dienstleister Seite aufrufen
   * @param provider
   */
  void s_queryProvider(const QString &provider);

public:
  explicit ProvidersTreeView(QWidget *parent = nullptr);

  /**
   * @brief Doppelte Einträge vermeiden
   * @param provider - Dienstleister
   * @param id       - Bestellnummer
   */
  bool exists(const QString &provider, const QString &id);

  /**
   * @brief Kopfdaten initialisieren
   */
  void setTreeViewHeaders();

  /**
   * @brief Dienstleister hinzufügen
   */
  void addProvider(const QString &);
};

#endif // INVENTORY_PROVIDERS_TREEVIEW_H
