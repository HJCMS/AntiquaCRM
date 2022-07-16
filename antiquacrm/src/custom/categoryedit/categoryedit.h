// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef CATEGORYEDIT_H
#define CATEGORYEDIT_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QSplitter>
#include <QStatusBar>
#include <QWidget>

#include <SqlCore>

class CategoryTree;
class DragListWidget;

class CategoryEdit : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *m_sql;
  QSplitter *m_splitter;
  CategoryTree *m_tree;
  DragListWidget *m_storageList;
  DragListWidget *m_keywordsList;
  QDialogButtonBox *m_btnBox;
  QPushButton *m_saveBtn;
  QStatusBar *m_statusBar;
  const int timeout = 6000;

  bool initCategories();
  bool initKeywords();

private Q_SLOTS:
  void saveCompanyTreeUsage();

private Q_SLOTS:
  void updateCompanyUsage(int categoryId, bool usage);
  void disableCompanyUsageList(const QStringList &ids);

public:
  explicit CategoryEdit(QWidget *parent = nullptr);
  int exec() override;
};

#endif // CATEGORYEDIT_H
