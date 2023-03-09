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

/**
 * @ingroup utils
 * @brief The TemplatesTree class
 */
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

public:
  explicit TemplatesTree(QWidget *parent = nullptr);
  bool clearTree();
  void addTemplates(const QJsonObject &);
};

#endif // TEMPLATES_TREE_H
