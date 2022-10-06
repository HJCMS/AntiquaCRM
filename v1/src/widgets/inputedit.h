// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INPUTEDIT_H
#define ANTIQUACRM_INPUTEDIT_H

#include <QComboBox>
#include <QFocusEvent>
#include <QFrame>
#include <QHBoxLayout>
#include <QMetaType>
#include <QObject>
#include <QVariant>
#include <QWheelEvent>
#include <QWidget>

class AntiquaComboBox final : public QComboBox {
  Q_OBJECT
  Q_PROPERTY(bool mouseWheel READ isMouseWheelEnabled WRITE setMouseWheelEnabled
                 NOTIFY sendMouseWheelChanged)

private:
  bool mouseWheel = false;

protected:
  void wheelEvent(QWheelEvent *e) override;
  void setMouseWheelEnabled(bool);

Q_SIGNALS:
  void sendMouseWheelChanged();

public:
  explicit AntiquaComboBox(QWidget *parent = nullptr);
  bool isMouseWheelEnabled();
};

class InputEdit : public QFrame {
  Q_OBJECT
  Q_PROPERTY(bool required /* Equivalent zu QSqlRecord:required */
                 READ isRequired WRITE setRequired NOTIFY sendRequireChanged)
  Q_PROPERTY(bool modified /* Ableitung für windowModified */
                 READ isModified WRITE setModified NOTIFY sendModifiedChanged)

private:
  bool required = false;
  bool modified = false;

protected:
  const Qt::Alignment labelAlign;
  QHBoxLayout *m_layout;
  virtual void loadDataset() = 0;

protected Q_SLOTS:
  virtual void skipReturnPressed();
  virtual void focusOutEvent(QFocusEvent *);

Q_SIGNALS:
  void sendHasModified(bool);
  void sendRequireChanged();
  void sendModifiedChanged();
  void sendLeaveEditor();

public Q_SLOTS:
  Q_INVOKABLE void setRequired(bool b);
  Q_INVOKABLE void setModified(bool b);
  Q_INVOKABLE virtual void reset() = 0;
  virtual void setValue(const QVariant &) = 0;
  virtual void setFocus() = 0;

public:
  explicit InputEdit(QWidget *parent = nullptr);
  Q_INVOKABLE bool isRequired();
  Q_INVOKABLE bool isModified();
  virtual const QVariant value() = 0;
  virtual bool isValid() = 0;
  virtual void setInfo(const QString &) = 0;
  virtual const QString info() = 0;
  virtual const QString notes() = 0;
};

typedef QList<InputEdit *> InputEditList;
Q_DECLARE_METATYPE(InputEditList)

#endif // ANTIQUACRM_INPUTEDIT_H