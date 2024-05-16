// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tablecontextmenu.h"
#include "antiquaicon.h"

#include <QDebug>

namespace AntiquaCRM
{

TableContextMenu::TableContextMenu(const QModelIndex& index, qint64 rowCount, QWidget* parent)
    : QMenu{parent}, p_index{index}, p_rows{rowCount} {
  p_modelList.clear();
  connect(this, SIGNAL(triggered(QAction*)), SLOT(triggerAction(QAction*)));
}

void TableContextMenu::triggerAction(QAction* action) {
  if (!p_index.isValid() || p_modelList.size() < 1)
    return;

  QListIterator<QAction*> it(p_modelList);
  while (it.hasNext()) {
    if (it.next() == action) {
      QString name = action->objectName();
      if (name == "ac_open") {
        emit sendAction(TableContextMenu::Actions::Open, p_index);
      } else if (name == "ac_add_order") {
        emit sendAction(TableContextMenu::Actions::Order, p_index);
      } else if (name == "ac_delete") {
        emit sendAction(TableContextMenu::Actions::Delete, p_index);
      } else if (name == "ac_copy") {
        emit sendAction(TableContextMenu::Actions::Copy, p_index);
      } else if (name == "ac_undo") {
        emit sendAction(TableContextMenu::Actions::Undo, p_index);
      } else if (name == "ac_custom") {
        emit sendAction(TableContextMenu::Actions::Custom, p_index);
      } else if (name == "ac_refund") {
        emit sendAction(TableContextMenu::Actions::Refund, p_index);
      } else {
        qWarning("Unknown Context Action");
        return;
      }
      break;
    }
  }
}

QAction* TableContextMenu::addOpenAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("server-database"), title);
  ac->setObjectName("ac_open");
  ac->setEnabled(p_index.isValid());
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addCreateAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("database-add"), title);
  ac->setObjectName("ac_create");
  ac->setEnabled(p_rows > 0);
  connect(ac, SIGNAL(triggered()), SIGNAL(sendCreate()));
  return ac;
}

QAction* TableContextMenu::addOrderAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("database-status"), title);
  ac->setObjectName("ac_add_order");
  ac->setEnabled(p_index.isValid());
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addDeleteAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("database-remove"), title);
  ac->setObjectName("ac_delete");
  ac->setEnabled(p_index.isValid());
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addCopyAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("database-comit"), title);
  ac->setObjectName("ac_copy");
  ac->setEnabled(p_index.isValid());
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addReloadAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("view-refresh"), title);
  ac->setObjectName("ac_reload");
  connect(ac, SIGNAL(triggered()), SIGNAL(sendRefresh()));
  return ac;
}

QAction* TableContextMenu::addUndoAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("action-undo"), title);
  ac->setObjectName("ac_undo");
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addRefundAction(const QString& title) {
  QAction* ac = addAction(antiquaIcon("action-redo"), title);
  ac->setObjectName("ac_refund");
  p_modelList.append(ac);
  return ac;
}

QAction* TableContextMenu::addCustomAction(const QIcon& icon, const QString& title) {
  QAction* m_ac = addAction(icon, title);
  m_ac->setObjectName("ac_custom");
  p_modelList.append(m_ac);
  return m_ac;
}

} // namespace AntiquaCRM
