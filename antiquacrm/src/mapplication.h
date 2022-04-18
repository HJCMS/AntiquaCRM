/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtWidgets/QApplication>

class ApplSettings;
class MWindow;

class MApplication : public QApplication {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  ApplSettings *m_settings;
  MWindow *m_mainWindow;

public:
  explicit MApplication(int &argc, char **argv);
  int exec();
  ~MApplication();
};

#endif // ANTIQUACRM_H
