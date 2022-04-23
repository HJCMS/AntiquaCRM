// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef TOOLBAR_IMAGING_H
#define TOOLBAR_IMAGING_H

#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QWidget>

/**
 Einfacher Toolbar mit einer ComboBox und
 einem PushButton für die Navigation.
*/
class ToolBar : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QComboBox *m_setTargets;

private Q_SLOTS:
  /**
     AuwahlBox für Wurzelverzeichnis
     wurde augewählt.
   */
  void targetChanged(int);

  /**
     Ein Verzeichnis nach oben gehen
     wurde ausgewählt.
   */
  void goUpClicked(bool);

Q_SIGNALS:
  /**
     AuwahlBox für Wurzelverzecihnisse
     wurde ausgeführt.
   */
  void goTo(const QString &);

  /**
     Ein Verzeichnis nach oben gehen
     wurde ausgeführt.
   */
  void goUp();

public:
  explicit ToolBar(QWidget *parent = nullptr);
};

#endif // TOOLBAR_IMAGING_H
