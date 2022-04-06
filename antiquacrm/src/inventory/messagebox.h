// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>

class MessageBox : public QMessageBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int timerID;
  int timeout;
  QString message;

private:
  void buildTimerMessage();

protected:
  void timerEvent(QTimerEvent *);

public:
  explicit MessageBox(QWidget *parent = nullptr);

  /**
     @brief queryFail
     @param QString Message
   */
  void queryFail(const QString &);

  /**
     @brief querySuccess
     @param QString Message
     @param s - timeout in seconds
   */
  void querySuccess(const QString &, int s = 10);
};

#endif // MESSAGEBOX_H
