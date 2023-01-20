// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_UPDATEDIALOG_H
#define ANTIQUACRM_UPDATEDIALOG_H

#include <AntiquaCRM>
#include <QDialog>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWidget>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY UpdateDialog : public QDialog {
  Q_OBJECT

protected:
  virtual bool prepareAction() = 0;

protected Q_SLOTS:
  virtual void networkAction() = 0;

public Q_SLOTS:
  virtual void statusMessage(const QString &) = 0;

Q_SIGNALS:
  void sendPluginAction(const QJsonObject &);

public:
  explicit UpdateDialog(QWidget *parent = nullptr);
  int exec() { return QDialog::exec(); };
  virtual int exec(const QString &, const QJsonObject &) = 0;
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_UPDATEDIALOG_H
