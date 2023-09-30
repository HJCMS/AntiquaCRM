// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_EDIT_H
#define PUBLISHERS_EDIT_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QEvent>
#include <QKeyEvent>
#include <QModelIndex>
#include <QObject>
#include <QStatusBar>
#include <QWidget>

class PublisherSearchbar;
class PublishersView;
class PublishersInput;

/**
 * @ingroup _publishersedit
 * @brief Book Publishers edit and selection
 */
class PublishersEdit final : public QDialog {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  PublisherSearchbar *m_searchBar;
  PublishersView *m_view;
  PublishersInput *m_input;
  QDialogButtonBox *m_buttonsBar;
  QStatusBar *m_statusBar;

private:
  void updateEntry(const QString &publisher);
  void insertEntry();
  void keyPressEvent(QKeyEvent *) override;
  bool event(QEvent *) override;

private Q_SLOTS:
  void rowSelected(const QModelIndex &);
  void prepareAction();

public:
  explicit PublishersEdit(QWidget *parent = nullptr);
  int exec() override;
};

#endif // PUBLISHERS_EDIT_H
