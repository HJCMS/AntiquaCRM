// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SYSTEMTRAY_H
#define ANTIQUACRM_SYSTEMTRAY_H

#include <QObject>
#include <QAction>
#include <QSystemTrayIcon>

class MWindow;

class AntiquaSystemTray : public QSystemTrayIcon {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QAction *ac_show;
  QAction *ac_hide;
  QAction *ac_quit;
  MWindow *m_window;

private Q_SLOTS:
  void toggleWindow();
  void actionReason(QSystemTrayIcon::ActivationReason r);

public Q_SLOTS:
  void notify(const QString &msg);
  void warning(const QString &msg);
  void fatal(const QString &msg);

public:
  explicit AntiquaSystemTray(const QIcon &icon, MWindow *parent = nullptr);
};

#endif // ANTIQUACRM_SYSTEMTRAY_H
