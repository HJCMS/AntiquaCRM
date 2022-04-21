// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPLETERDIALOG_H
#define COMPLETERDIALOG_H

#include <QtCore/QObject>
#include <QtWidgets/QDialog>

class CompleterView;

class CompleterDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  CompleterView *m_tableView;

public:
  explicit CompleterDialog(QWidget *parent = nullptr,
                          const QString &filter = QString("IS NOT NULL"));
};

#endif // COMPLETERDIALOG_H
