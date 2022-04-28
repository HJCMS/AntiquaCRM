/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUACRM_MAPPLICATION_H
#define ANTIQUACRM_MAPPLICATION_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtNetwork/QLocalServer>
#include <QtWidgets/QApplication>

class ApplSettings;
class MWindow;
class SocketServer;

class MApplication : public QApplication {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *m_settings;
  MWindow *m_mainWindow;
  SocketServer *m_socket;
  bool initialSocketServer();

public:
  explicit MApplication(int &argc, char **argv);
  bool isRunning();
  int exec();
  virtual ~MApplication();
};

#endif // ANTIQUACRM_H
