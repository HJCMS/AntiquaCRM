#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtSql/QSqlDatabase>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

namespace HJCMS {
class SqlCore;
};

class StatusBar : public QStatusBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  /**
    @brief Verbindung für Listener!
  */
  HJCMS::SqlCore *m_sql;

  /**
    @brief Datenbankstatus
    Zeigt den aktuellen Datenbank Status
    in einem ToolButton an.
    @todo  Bei onclick Status Widget öffnen!
  */
  QToolButton *m_dbStatus;

private Q_SLOTS:
  void setDatabaseStatusIcon(bool b = false);

protected:
  /**
   @brief Verzögerung bis @ref m_sql gestartet wird!
  */
  void timerEvent(QTimerEvent *){};

public Q_SLOTS:
  void sqlStatusMessage(const QString &);

public:
  explicit StatusBar(QWidget *parent = 0);
  virtual ~StatusBar();
};

#endif // STATUSBAR_H
