// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef MESSAGEBOX_UTILS_H
#define MESSAGEBOX_UTILS_H

#include <QObject>
#include <QTimerEvent>
#include <QMessageBox>
#include <QWidget>

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
     @brief notice
     @param QString Message
   */
  int notice(const QString &);

  /**
     @brief queryFail
     @param QString Message
   */
  int failed(const QString &err, const QString &details = QString());

  /**
     @brief querySuccess
     @param QString Message
     @param s - timeout in seconds
   */
  int success(const QString &, int s = 10);
};

#endif // MESSAGEBOX_UTILS_H
