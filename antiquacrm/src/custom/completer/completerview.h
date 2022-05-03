// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPLETERVIEW_H
#define COMPLETERVIEW_H

#include <QObject>
#include <QContextMenuEvent>
#include <QSqlDatabase>
#include <QShortcut>
#include <QTableView>
#include <QWidget>

class DataTableModel;

class CompleterView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QSqlDatabase p_db;
  const QString p_type;
  DataTableModel *m_model;
  QShortcut *m_addShortcut;

private Q_SLOTS:
  void errorPopUp(const QString &);
  void createNewEntry();
  void removeSelection();

protected:
  void contextMenuEvent(QContextMenuEvent *ev);

//Q_SIGNALS:
//public Q_SLOTS:

public:
  explicit CompleterView(const QString &filter, QWidget *parent = nullptr);
};

#endif // COMPLETERVIEW_H
