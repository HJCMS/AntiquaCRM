// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABECATALOG_H
#define ABECATALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QObject>
#include <QStackedWidget>
#include <QString>

/**
 * @brief AbeCatalog Dialog
 */
class AbeCatalog final : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QStackedWidget *m_stackedWidget;
  QDialogButtonBox *m_btnBox;

private Q_SLOTS:
  void testBuilder();
  void testReader();

public:
  explicit AbeCatalog(QWidget *parent = nullptr);
  int exec() override;
};

#endif // ABECATALOG_H
