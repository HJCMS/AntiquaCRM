// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

class ToolBar : public QToolBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_setTargets;

private Q_SLOTS:
  void targetChanged(int);
  void goUpClicked(bool);

Q_SIGNALS:
  void goUp(); /**< e.g. QDir::cdUp() */
  void goTo(const QString &);

public:
  explicit ToolBar(QWidget *parent = nullptr);
};

#endif // TOOLBAR_H
