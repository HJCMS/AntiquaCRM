// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TEMPLATES_TREE_H
#define TEMPLATES_TREE_H

#include <AntiquaCRM>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class TemplatesTree final : public QTreeWidget {
  Q_OBJECT

private:
  QTreeWidgetItem *getParent(const QString &id);
  void createCategories();
  void insertTemplate(const QJsonObject &);

private Q_SLOTS:
  void templateClicked(QTreeWidgetItem *, int);

Q_SIGNALS:
  void templateSelected(const QJsonObject &);

public Q_SLOTS:
  void clearTree();

public:
  explicit TemplatesTree(QWidget *parent = nullptr);
  void addTemplates(const QJsonObject &);
};

#endif // TEMPLATES_TREE_H