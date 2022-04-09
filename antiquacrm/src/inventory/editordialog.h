// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QtCore/QObject>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

class EditorDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QVBoxLayout *m_layout;
  QDialogButtonBox *m_buttonBox;

protected:
  /**
   @brief keyPressEvent
   Damit keine Daten aus versehen Gespeichert werden!
   Die Eingabe von Key::{Return,Enter} unterbinden!
  */
  void keyPressEvent(QKeyEvent *);

protected Q_SLOTS:
  void saveClicked();
  void buttonClicked(QAbstractButton *);

Q_SIGNALS:
  void s_commitDataset();
  void s_restoreDataset();

public:
  explicit EditorDialog(QWidget *parent = nullptr);
  void setMainWidget(QWidget *);
};

#endif // EDITORDIALOG_H
