// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "templatestreelist.h"

#include <AntiquaWidgets>
#include <QJsonArray>

TemplatesTreeList::TemplatesTreeList(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(2);
  setHeaderLabels(QStringList({tr("Title"), tr("Template")}));
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setAcceptDrops(false);
  setDragEnabled(false);
  createCategories();
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)),
          SLOT(templateClicked(QTreeWidgetItem *, int)));
}

QTreeWidgetItem *TemplatesTreeList::getParent(const QString &id) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *parent = topLevelItem(i);
    if (parent != nullptr && parent->type() == QTreeWidgetItem::Type) {
      QString section = parent->data(0, Qt::UserRole).toString();
      if (section == id)
        return parent;
    }
  }
  return nullptr;
}

void TemplatesTreeList::createCategories() {
  QTreeWidgetItem *i = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  i->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  i->setFlags(Qt::ItemIsEnabled);
  i->setText(0, tr("eMail"));
  i->setData(0, Qt::DecorationRole, AntiquaCRM::antiquaIcon("mail-read"));
  i->setData(0, Qt::UserRole, QString("email"));
  i->setExpanded(true);
  addTopLevelItem(i);

  i = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  i->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  i->setFlags(Qt::ItemIsEnabled);
  i->setText(0, tr("Printing"));
  i->setData(0, Qt::DecorationRole, AntiquaCRM::antiquaIcon("printer"));
  i->setData(0, Qt::UserRole, QString("printing"));
  i->setExpanded(true);
  addTopLevelItem(i);

  resizeColumnToContents(0);
}

void TemplatesTreeList::insertTemplate(const QJsonObject &data) {
  QTreeWidgetItem *parent = getParent(data.value("tb_category").toString());
  if (parent == nullptr)
    return;

  QIcon _icon = AntiquaCRM::antiquaIcon("document-edit");
  QFont _font = font();
  _font.setStyleHint(QFont::Serif, QFont::PreferDefault);
  _font.setStyle(QFont::StyleItalic);

  QTreeWidgetItem *i = new QTreeWidgetItem(parent, QTreeWidgetItem::UserType);
  i->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |
              Qt::ItemNeverHasChildren);

  i->setText(0, data.value("tb_title").toString());
  i->setData(0, Qt::DecorationRole, _icon);
  i->setFont(1, _font);
  i->setForeground(1, QBrush(Qt::gray));
  i->setText(1, data.value("tb_caller").toString());
  i->setData(1, Qt::UserRole, data);
  parent->addChild(i);
}

void TemplatesTreeList::templateClicked(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column);
  if (item->type() != QTreeWidgetItem::UserType)
    return;

  if (item->flags() == Qt::NoItemFlags)
    return;

  QJsonObject _obj = item->data(1, Qt::UserRole).toJsonObject();
  if (_obj.isEmpty())
    return;

  emit sendTemplate(_obj);
}

bool TemplatesTreeList::cleanup() {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *p = topLevelItem(i);
    if (p == nullptr)
      continue;

    QListIterator<QTreeWidgetItem *> it(p->takeChildren());
    while (it.hasNext()) {
      p->removeChild(it.next());
    }
  }
  return true;
}

void TemplatesTreeList::addTemplates(const QJsonObject &data) {
  QJsonArray _arr = data.value("tree").toArray();
  for (int i = 0; i < _arr.size(); i++) {
    QJsonObject _obj = _arr[i].toObject();
    if (!_obj.isEmpty())
      insertTemplate(_obj);
  }
  resizeColumnToContents(0);
}
