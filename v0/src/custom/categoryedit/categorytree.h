// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_TREE_H
#define CATEGORYEDIT_TREE_H

#include <QEvent>
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

#include "categorymapping.h"

class TreeKeywordItem;

/**
 * @brief Baumansicht für Kategorien
 * @ingroup CategoryEdit
 */
class CategoryTree : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Temporärer Status Variable für Baum öffnen/schließen.
   */
  bool p_expandet = false;

  /**
   * @brief Temporärer Status Deaktivierte ausblenden!
   */
  bool p_hide = false;

  /**
   * @brief Wartezeit bis ein Element beim Drüberschieben aufgeklappt wird.
   */
  int p_timerWait = 1000;

  /**
   * @brief Temporäre Timer ID
   * Wird verwendet wenn bei einem Drag ein geschlossenes Element Automatisch
   * aufklappen soll! Siehe timerEvent und dragMoveEvent
   * Soll verhindern das die Methode expandOnDragHover einen Overload
   * verursacht!
   */
  int p_expandTimerId = -1;

  /**
   * @brief Datentype der für das interne Verschieben gültig ist.
   */
  QByteArray itemMime = QByteArray("application/x-qabstractitemmodeldatalist");

  /**
   * @brief Liste der Element die gerade verschoben werden!
   */
  QPair<QTreeWidgetItem *, QTreeWidgetItem *> p_sDrag;

  /**
   * @brief Beim verschieben das Baumelement prüfen.
   * Wenn es aufgeklappt werden kann, true zurückgeben.
   */
  inline bool isExpandable(QTreeWidgetItem *item) const;

  /**
   * @brief Suche das Eltern Objekt
   */
  QTreeWidgetItem *findParent(int id);

  /**
   * @brief Standard Icon für Schlüsselwort Aktiv/Deaktiviert!
   */
  const QIcon setIcon(bool b = true) const;

private Q_SLOTS:
  /**
   * @brief Ein Element aufklappen wenn die Maus über im stehen bleibt!
   */
  void expandOnDragHover();

  /**
   * @brief Wird vom Kontext Menü ausgelöst.
   * Alle Elemente von diesem Eltern Element deaktivieren!
   */
  void toggleParentsTree(bool);

  /**
   * @brief Kontext Menü Element Aktivieren/Deaktivieren.
   */
  void toggleActivation(bool);

  /**
   * @brief Schlüsselwort entfernen
   * Bei erfolg wird Signal sendListItemVisible ausgelöst.
   */
  void removeKeyword(bool);

  /**
   * @brief Ein Schlüsselwort wird durch Drop hinzugefügt.
   */
  bool addKeywordItem(QTreeWidgetItem *parent, const QString &name);

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void timerEvent(QTimerEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

Q_SIGNALS:
  /**
   * @brief Nach erfolgreichen einfügen Signal an DragListWidget
   */
  void sendListItemHidden(const QString &name);

  /**
   * @brief Nach erfolgreichen entfernen Signal an DragListWidget
   */
  void sendListItemVisible(const QString &name);

  /**
   * @brief Kategorie in der Datenbank Aktivieren/Deaktivieren
   */
  void sendCompanyUsage(int categoryId, bool active);

  /**
   * @brief Kategorieliste in der Datenbank Deaktivieren
   * @param ids Kategorien ID's Liste
   */
  void sendDisableUsageList(const QStringList &ids);

public Q_SLOTS:
  /**
   * @brief Umsetzung für expandAll() / collapseAll()
   */
  void toggleTreeView();

  /**
   * @brief Deaktivierte Ein/Ausblenden.
   */
  void toggleVisible();

  /**
   * @brief Schlüsselwort hervorheben
   */
  void findKeyword(const QString &);

public:
  explicit CategoryTree(QWidget *parent = nullptr);

  /**
   * @brief Hauptkategorien einfügen
   */
  QTreeWidgetItem *addTopLevel(const QString &name, int zvab = 0);

  /**
   * @brief Unterkategorien einfügen
   */
  QTreeWidgetItem *addSubLevel(const QString &name, int bind, int ceId,
                               const QStringList &keywords, bool active = true);

  /**
   * @brief Alle Schlüsselwort vom Baum auslesen.
   */
  const QStringList getCurrentKeywords();

  /**
   * @brief Liste für Speicherfunktione ziehen
   */
  const CategoryMappingList getMapppings();
};

#endif // CATEGORYEDIT_TREE_H
