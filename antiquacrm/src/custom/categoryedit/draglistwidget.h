// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_LIST_H
#define CATEGORYEDIT_LIST_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

/**
 * @brief The DragListWidget class
 * @ingroup CategoryEdit
 */
class DragListWidget : public QListWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  void mousePressEvent(QMouseEvent *event) override;

public Q_SLOTS:
  /**
   * @brief Element Ausblenden
   */
  void setItemHidden(const QString &name);

  /**
   * @brief Element Einblenden
   */
  void setItemVisible(const QString &name);

public:
  explicit DragListWidget(QWidget *parent = nullptr);
  void setItemsHidden(const QStringList &list);
};

#endif // CATEGORYEDIT_LIST_H
