// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_TREE_H
#define CATEGORYEDIT_TREE_H

#include <QEvent>
#include <QIcon>
#include <QMouseEvent>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class CategoryTree : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QTreeWidgetItem *findParent(int id);
  const QIcon setIcon(bool b = true) const;

private Q_SLOTS:
  void toggleSubTree(bool);
  void toggleActivation(bool);
  void removeKeyword(bool);
  bool addKeywordItem(QTreeWidgetItem *parent, const QString &name);

protected:
  void contextMenuEvent(QContextMenuEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dragMoveEvent(QDragMoveEvent *event) override;
  void dropEvent(QDropEvent *event) override;

Q_SIGNALS:
  void sendCompanyUsage(int categoryId, bool active);
  void sendDisableUsageList(const QStringList &ids);

public:
  explicit CategoryTree(QWidget *parent = nullptr);
  QTreeWidgetItem *addTopLevel(const QString &name);
  QTreeWidgetItem *addSubLevel(const QString &name, int bind,
                               bool active = true);
};

#endif // CATEGORYEDIT_TREE_H
