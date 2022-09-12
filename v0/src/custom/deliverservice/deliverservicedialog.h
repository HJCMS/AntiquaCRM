// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICEDIALOG_H
#define DELIVERSERVICEDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QJsonObject>
#include <QKeyEvent>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QWidget>

class DeliverServiceEdit;
class DeliverServiceList;

#include <SqlCore>

class DeliverServiceDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QSplitter *m_splitter;
  DeliverServiceEdit *m_edit;
  DeliverServiceList *m_list;
  QDialogButtonBox *m_buttonBox;
  QPushButton *ac_create;
  QStatusBar *m_statusBar;

  void initItemsTreeView();

private Q_SLOTS:
  void messanger(const QString &);
  void deleteDeliveryPackage(const QJsonObject &obj);

protected:
  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void saveCurrent();
  void queryDeliverServices(int);
  void queryServicePackage(int, const QJsonObject &);

public:
  explicit DeliverServiceDialog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // DELIVERSERVICEDIALOG_H
