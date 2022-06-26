// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICEDIALOG_H
#define DELIVERSERVICEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QSplitter>
#include <QStatusBar>
#include <QWidget>

class DeliverService;
class DeliverServiceList;

#include <SqlCore>

class DeliverServiceDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QSplitter *m_splitter;
  DeliverService *m_view;
  DeliverServiceList *m_list;
  QDialogButtonBox *m_buttonBox;
  QStatusBar *m_statusBar;


  void initItemsTreeView();

private Q_SLOTS:
  void queryDeliverServices(int);
  void queryServicePackage(int, const QString &);

public:
  explicit DeliverServiceDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // DELIVERSERVICEDIALOG_H
