/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QLockFile>
#include <QtCore/QObject>
/* QtWidgets */
#include <QtWidgets/QApplication>
/* QtSQL */
#include <QtSql/QSqlDatabase>
/* Project */
#include "mwindow.h"

namespace HJCMS {
 class SqlCore;
}

class MApplication : public QApplication {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  MWindow *m_mainWindow;
  HJCMS::SqlCore *m_db;
  bool initDatabase();
  QLockFile sessionLock;

Q_SIGNALS:
  void postMessage(const QString &);

public:
  explicit MApplication(int &argc, char **argv);
  int exec();
  ~MApplication();
};

#endif // ANTIQUACRM_H
