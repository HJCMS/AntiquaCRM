// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPLETERTABLE_H
#define COMPLETERTABLE_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTimerEvent>
#include <QtGui/QContextMenuEvent>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtWidgets/QDialog>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QTableView>

class DataTableModel;

class CompleterTable : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QSqlDatabase p_db;
  QTableView *m_tableView;
  DataTableModel *m_model;
  QShortcut *m_addShortcut;
  QShortcut *m_delShortcut;

private Q_SLOTS:
  void removeSelectedEntries();

protected:
  void contextMenuEvent(QContextMenuEvent *ev);

public:
  explicit CompleterTable(QWidget *parent = nullptr,
                          const QString &filter = QString("IS NOT NULL"));
};

#endif // COMPLETERTABLE_H
