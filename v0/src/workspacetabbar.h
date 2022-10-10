// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WORKSPACE_TABBAR_H
#define WORKSPACE_TABBAR_H

#include <QContextMenuEvent>
#include <QEvent>
#include <QObject>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

/**
 * @class WorkspaceTabBar
 * @ingroup Inventory
 * @brief Registerkarte des Arbeistbereichs
 */
class WorkspaceTabBar final : public QTabBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int index = -1;

protected:
  /**
   * @brief Wenn eine neue Regfisterkarte eingefügt wird!
   */
  void tabInserted(int index) override;

  /**
   * @brief Menu für Registerkarte schließen.
   */
  void contextMenuEvent(QContextMenuEvent *event) override;

  /**
   * @brief Deaktiviere Mausrad
   * Manche Helferfunktionen sind bei Älteren nutzern nicht Hilfreich!
   */
  void wheelEvent(QWheelEvent *event) override;

protected Q_SLOTS:
  void tabIndexChanged(int);

  /**
   * @brief Prüfung bevor Signal sendCloseTabe gesendet wird!
   */
  void checkToClose();

Q_SIGNALS:
  void sendCloseTab(int index);

public:
  explicit WorkspaceTabBar(QWidget *parent = nullptr);
};

#endif // WORKSPACE_TABBAR_H
