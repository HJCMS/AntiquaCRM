// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QObject>
#include <QSqlDatabase>
#include <QStatusBar>
#include <QTimerEvent>
#include <QToolButton>
#include <QWidget>

namespace HJCMS {
class SqlCore;
};

class StatusBar : public QStatusBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
   * @brief Verbindung für Listener!
   */
  HJCMS::SqlCore *m_sql;

  /**
   * @brief Datenbankstatus
   * Zeigt den aktuellen Datenbank Status in einem ToolButton an.
   * @todo  Bei onclick Status Widget öffnen!
   */
  QToolButton *m_dbStatus;

protected:
  /**
   * @brief Verzögerung bis @ref HJCMS::SqlCore gestartet wird!
   */
  void timerEvent(QTimerEvent *){};

public Q_SLOTS:
  void sqlStatusMessage(const QString &);
  void setDatabaseStatusIcon(bool b = false);

public:
  explicit StatusBar(QWidget *parent = 0);
  virtual ~StatusBar();
};

#endif // STATUSBAR_H
